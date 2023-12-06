fn hello_world() {
    let southern_germany = "Gott Grüß!";
    let chinese = "你好，世界！";
    let english = "Hello World!";
    let regions = [southern_germany, chinese, english];
    for region in regions {
        println!("{}", &region);
    }
}

fn main() {
    // Comments
    hello_world();
}