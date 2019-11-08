;;; === Windows 7/10 Simplified Chinese ===
;;; Set the default shell to be gitbash for Windows.
;(setq explicit-shell-file-name
;      "/usr/bin/bash")
;      ; "C:/Windows/System32/bash.exe")
;;;
;(setq shell-file-name explicit-shell-file-name)
;
;;;(set-language-environment "UTF-8")
;;;(set-locale-environment "UTF-8")
;
;;; Display Chinese characters correctly in "M-! dir". 
;(prefer-coding-system 'utf-8)
;(setq file-name-coding-system 'gbk)
;(set-terminal-coding-system 'gbk)
;(set-keyboard-coding-system 'gbk)
;(setq locale-coding-system 'gbk)
;(set-selection-coding-system 'gbk)
;(set-clipboard-coding-system 'ctext)
;(set-clipboard-coding-system 'gbk)
;(set-terminal-coding-system 'gbk)
;(set-buffer-file-coding-system 'gbk)
;(modify-coding-system-alist 'process "." 'gbk)
;(setq default-process-coding-system '(gbk . gbk))

;; === Packages ===
;; Add the following three lines to enable package install.
;; then M-x package-list-packages to list and install ox-gfm.
;; then add the fourth line: (require 'ox-gfm).
;; http://orgmode.org/manual/Installation.html#Installation
(require 'package)
(add-to-list 'package-archives
     	     '("melpa" . "http://melpa.milkbox.net/packages/") t)
(add-to-list 'package-archives
              '("org" . "http://orgmode.org/elpa/") t)
(package-initialize)
(require 'ox-gfm)

;; Jekyll markdown/html
(add-to-list 'auto-mode-alist '("\\.md$" . jekyll-markdown-mode))
(add-to-list 'auto-mode-alist '("\\.html" . jekyll-html-mode))

;; === Proxy ===
(setq url-proxy-services
       '(("no_proxy" . "^\\(localhost\\|10.*\\)")
         ("http" . "http://127.0.0.1:12639")
         ("https" . "http://127.0.0.1:12639")))

;; === Org publish ===
;; http://orgmode.org/worg/org-tutorials/org-publish-html-tutorial.html
(require 'ox-publish)
(setq org-publish-project-alist
  '(
    ("org-notes"
     :base-directory "~/workspace/org"
     :base-extension "org"
     :publishing-directory "~/workspace/publish"
     :recursive t
     :publishing-function org-html-publish-to-html
     :headline-levels 4  ; Just the default for this project.
     :auto-preamble t
     )  
    ("org-static"
    :base-directory "~/workspace/org"
    :base-extension "css\\|js\\|png\\|jpg\\|gif\\|pdf\\|mp3\\|ogg\\|swf"
    :publishing-directory "~/workspace/publish"
    :recursive t
    :publishing-function org-publish-attachment
    )
    ("org" :components ("org-notes" "org-static"))
   ))

;; https://zilongshanren.com/blog/2015-07-19-add-org-mode-support.html#orgheadline11
(defadvice org-html-paragraph
  (before org-html-paragraph-advice (paragraph contents info) activate)
  "Join consecutive Chinese lines into a single long line without unwanted space when exporting org-mode to html."
  (let* ((origin-contents (ad-get-arg 1))
    (fix-regexp "[[:multibyte:]]")
    (fixed-contents (replace-regexp-in-string
    (concat "\\(" fix-regexp "\\) *\n *\\(" fix-regexp "\\)") "\\1\\2" origin-contents)))
    (ad-set-arg 1 fixed-contents)))

;; http://ergoemacs.org/emacs/modernization_fill-paragraph.html
(defun xah-fill-or-unfill ()
  "Reformat current paragraph or region to `fill-column', like `fill-paragraph' or “unfill”.
When there is a text selection, act on the selection, else, act on a text block separated by blank lines.
URL `http://ergoemacs.org/emacs/modernization_fill-paragraph.html'
Version 2017-01-08"
  (interactive)
  ;; This command symbol has a property “'compact-p”, the possible values are t and nil. This property is used to easily determine whether to compact or uncompact, when this command is called again
  (let ( ($compact-p
          (if (eq last-command this-command)
              (get this-command 'compact-p)
            (> (- (line-end-position) (line-beginning-position)) fill-column)))
         (deactivate-mark nil)
         ($blanks-regex "\n[ \t]*\n")
         $p1 $p2
         )
    (if (use-region-p)
        (progn (setq $p1 (region-beginning))
               (setq $p2 (region-end)))
      (save-excursion
        (if (re-search-backward $blanks-regex nil "NOERROR")
            (progn (re-search-forward $blanks-regex)
                   (setq $p1 (point)))
          (setq $p1 (point)))
        (if (re-search-forward $blanks-regex nil "NOERROR")
            (progn (re-search-backward $blanks-regex)
                   (setq $p2 (point)))
          (setq $p2 (point)))))
    (if $compact-p
        (fill-region $p1 $p2)
      (let ((fill-column most-positive-fixnum ))
        (fill-region $p1 $p2)))
    (put this-command 'compact-p (not $compact-p))))

;; === Latex ===
;; Use xelatex instead of pdflatex since the latter does not work well
;; for Chinese characters.
(setq org-latex-pdf-process 
    '("xelatex -interaction nonstopmode -output-directory %o %f"
	  "xelatex -interaction nonstopmode -output-directory %o %f"
	  "xelatex -interaction nonstopmode -output-directory %o %f"))

;; Customize the latex template
;; Still need improvements
(unless (boundp 'org-latex-classes)
  (setq org-latex-classes nil))
(require 'ox-latex)  
(add-to-list 'org-latex-classes
             '("article-zh"
               "\\documentclass[11pt]{article}\n\\usepackage{fontspec}\n\\usepackage{geometry}\n\\usepackage{hyperref}\n\\setmainfont{Microsoft Yahei}\n\\setsansfont{Microsoft Yahei}\n\\setmonofont{Microsoft Yahei}\n\\geometry{a4paper, textwidth=6.5in, textheight=10in,marginparsep=7pt, marginparwidth=.6in}\n\\author{Wenliang Zhang}\n[NO-DEFAULT-PACKAGES][NO-PACKAGES]"
               ("\\section{%s}" . "\\section*{%s}")
               ("\\subsection{%s}" . "\\subsection*{%s}")
               ("\\subsubsection{%s}" . "\\subsubsection*{%s}")
               ("\\paragraph{%s}" . "\\paragraph*{%s}")
               ("\\subparagraph{%s}" . "\\subparagraph*{%s}")))

;; === Misc ===
;; Force wrap at column 80
(require 'whitespace) 
(setq whitespace-line-column 80)

;; If turned on, M-x whitespace-cleanup won't work
(setq whitespace-style '(face lines-tail)) 

(add-hook 'org-mode-hook 'whitespace-mode)
(add-hook 'org-mode-hook 'ruler-mode)
(add-hook 'text-mode-hook '(lambda() (turn-on-auto-fill) (set-fill-column 80)))

(setq column-number-mode t)
(setq make-backup-files nil)
(setq package-selected-packages (quote (ox-gfm org-plus-contrib org html2org whitespace-cleanup-mode poly-markdown jekyll-modes markdown-preview-mode html-to-markdown ox-pandoc htmlize markdown-modee)))
(setq savehist-mode t)
(setq show-paren-mode t)
(setq tab-width 2)
(setq tool-bar-mode nil)

;; Wrap at Window Edge for org-mode 
(setq org-startup-truncated nil)

;; http://stackoverflow.com/questions/1322591/tracking-down-max-specpdl-size-errors-in-emacs
(setq max-specpdl-size 1000) 

;; Show column number
(setq column-number-mode t)

;; Set find, grep etc.
(require 'grep)
(setq grep-program "/usr/bin/grep")
(setq find-program "/usr/bin/find")
(setq markdown-command "/usr/local/bin/markdown")

;; single instance
(require 'server)
(unless (server-running-p)
  (server-start))

(put 'erase-buffer 'disabled nil)

;; Max the main window when emacs startup
(toggle-frame-maximized) 

;; Wrap at Window Edge for org-mode 
(setq org-startup-truncated nil)

;; Auto indent for org-mode
; (setq org-startup-indented t)
(setq inhibit-startup-screen t)
(setq initial-frame-alist (quote ((fullscreen . maximized))))
(setq org-startup-truncated nil)
(setq package-enable-at-startup nil)

(put 'upcase-region 'disabled nil)
(put 'downcase-region 'disabled nil)

;; Black background and white foreground etc.
(add-hook 'emacs-startup-hook
  (lambda ()
    (load-theme 'wheatgrass)
    ))

;; MacOS default font is Menlo 12, Microsoft Yahei can also be used.
(if (version< "27.0" emacs-version)
  (set-frame-font "Menlo 14")
  (set-default-font "Menlo 14"))

;; https://github.crookster.org/emacs27-from-homebrew-on-macos-with-emoji/
;; Install Emacs 27 from source code by homebrew
(if (version< "27.0" emacs-version)
  (set-fontset-font "fontset-default" 'unicode "Apple Color Emoji" nil 'prepend)
  (set-fontset-font t 'symbol (font-spec :family "Apple Color Emoji") nil 'prepend))

