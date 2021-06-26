package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"os"
)

const (
	headerSize      = 14 + 40 // standard size of bmp file header
	lengthFieldSize = 32      // size of occupancy in bmp for the length of hidden data
	infoUnitSize    = 4       // size of occupancy in bmp for a byte of hidden data
)

// hides an integer to a byte array
func hide(txt int, pix []byte, size int) {
	for i := 0; i < size; i++ {
		x := int(pix[i])
		y := txt
		// 0xFC=1111,1100
		x = x & 0xFC
		// 0x3=11
		y = y & (0x3 << (2 * i))
		y = y >> (2 * i)
		pix[i] = byte(x | y)
	}
}

// shows the hidden byte in a byte array
func show(j int, t []byte, pix []byte, size int) {
	x := int(0)
	for i := 0; i < size; i++ {
		y := (int(pix[i]) & 0x3) << (2 * i)
		x = x | y
	}

	t[j] = byte(x)
}

var (
	encodeOrDecode bool
	srcImage       string // input image path
	txtFile        string // input text file path
	destFile       string // output image file for encode, output text file for decode
)

// golang's boolean flag is weird:
// https://stackoverflow.com/questions/27411691/how-to-pass-boolean-arguments-to-go-flags/27411724
// https://golang.org/pkg/flag/
func init() {
	flag.BoolVar(&encodeOrDecode, "f", true, "true for encode, false for decode, missing -f defaults to true. \nand -f=false means false, -f false or -f 0 is incorrect.\nSee https://stackoverflow.com/questions/27411691/how-to-pass-boolean-arguments-to-go-flags/27411724\nAlso see https://golang.org/pkg/flag/ .")
	flag.StringVar(&srcImage, "i", "", "input image path.")
	flag.StringVar(&txtFile, "t", "", "input text file path.")
	flag.StringVar(&destFile, "o", "", "output image or text file path.")
}

func encode(srcImage string, txtFile string, destImage string) {
	// read input image to a byte array p
	p, err := ioutil.ReadFile(srcImage)
	if err != nil {
		fmt.Printf("Open image file failed, err = %v\n", err)
		os.Exit(1)
	}

	// read input text file to a byte array t
	t, err := ioutil.ReadFile(txtFile)
	if err != nil {
		fmt.Printf("Open text file failed, err = %v\n", err)
		os.Exit(1)
	}

	// check if the image can contain the content of the text
	offset := headerSize
	if offset >= len(p) {
		fmt.Println("File size too small, is it a valid bmp file?")
		os.Exit(1)
	}

	if lengthFieldSize+len(t)*infoUnitSize > len(p[offset:]) {
		fmt.Println("The content of text is too long")
		os.Exit(1)
	}

	// save the text length to p
	hide(len(t), p[offset:], lengthFieldSize)
	offset += lengthFieldSize

	// save the content of text to p
	for i := 0; i < len(t); i++ {
		offset += infoUnitSize
		hide(int(t[i]), p[offset:], infoUnitSize)
	}

	// save the modified p to destImage
	err = ioutil.WriteFile(destImage, p, 0666)
	if err != nil {
		fmt.Printf("Save doctored image failed, err = %v\n", err)
		os.Exit(1)
	}
}

func decode(image string, txtFile string) {
	// read input image to a byte array p
	p, err := ioutil.ReadFile(image)
	if err != nil {
		fmt.Printf("Open image file failed, err = %v\n", err)
		os.Exit(1)
	}

	// check if the image can contain the content of the text
	offset := headerSize
	if offset >= len(p) {
		fmt.Println("File size too small, is it a valid bmp file?")
		os.Exit(1)
	}

	// compute length
	var length int
	length = 0
	for j := 0; j < lengthFieldSize; j++ {
		length = ((int(p[j+offset]) & 0x3) << (2 * j)) | length
	}

	var t []byte = make([]byte, length)
	offset = headerSize + lengthFieldSize
	// save the shown text to t
	for i := 0; i < length; i++ {
		offset += infoUnitSize
		show(i, t, p[offset:], infoUnitSize)
	}

	// save the modified t to txtFile
	err = ioutil.WriteFile(txtFile, t, 0666)
	if err != nil {
		fmt.Printf("Save shown text failed, err = %v\n", err)
		os.Exit(1)
	}
}

func main() {
	flag.Parse()
	// fmt.Printf("f=%v i=%s t=%s o=%s\n", encodeOrDecode, srcImage, txtFile, destFile)

	if encodeOrDecode {
		if srcImage == "" || txtFile == "" || destFile == "" {
			flag.PrintDefaults()
			os.Exit(1)
		}

		if srcImage == destFile {
			fmt.Println("source image can't be same as destination file.")
			os.Exit(1)
		}

		encode(srcImage, txtFile, destFile)
	} else {
		if srcImage == "" || txtFile != "" || destFile == "" {
			flag.PrintDefaults()
			os.Exit(1)
		}

		if srcImage == destFile {
			fmt.Println("source image can't be same as destination file.")
			os.Exit(1)
		}

		decode(srcImage, destFile)
	}
}
