;; !!!
;; @author: William Zhang
;; @os: Windows 7 Simplified Chinese
;; Do not edit this file in emacs, since it will convert the default
;; encoding to UTF-8 and cause the convertion of PDF files failed.
;; !!!

;; Customize the fonts. Looks good for English and Simplified Chinese.
;; For different display resolutions, may need different font size. 

;; Added by Package.el.  This must come before configurations of
;; installed packages.  Don't delete this line.  If you don't want it,
;; just comment it out by adding a semicolon to the start of the line.
;; You may delete these explanatory comments.
(package-initialize)

;; Cutomized Font setting
(if (not (member '("-*-consolas-normal-r-*-*-18-*-*-*-c-*-fontset-chinese"
   . "fontset-chinese") fontset-alias-alist))
     (progn
       (create-fontset-from-fontset-spec
        "-*-consolas-normal-r-*-*-18-*-*-*-c-*-fontset-chinese,
       chinese-gb2312:-*-Microsoft Yahei-normal-r-*-*-18-*-*-*-c-*-gb2312*-*,
       chinese-big5-1:-*-MingLiU-normal-r-*-*-18-*-*-*-c-*-big5*-*,
       chinese-big5-2:-*-MingLiU-normal-r-*-*-18-*-*-*-c-*-big5*-*" t)
       (setq default-frame-alist
             (append
              '((font . "fontset-chinese"))
              default-frame-alist))
       )
)

;; Cutomize settings generated.
(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(column-number-mode t)
 '(custom-enabled-themes (quote (manoj-dark)))
 '(make-backup-files nil)
 '(package-selected-packages (quote (ox-gfm org-plus-contrib org)))
 '(savehist-mode t)
 '(show-paren-mode t)
 '(tab-width 4)
 '(tool-bar-mode nil))

(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(default ((t (:family "微软雅黑" :foundry "outline" :slant normal :weight normal :height 102 :width normal)))))

;; Wrap at Window Edge for org-mode 
(setq org-startup-truncated nil)

;; http://stackoverflow.com/questions/1322591/tracking-down-max-specpdl-size-errors-in-emacs
(setq max-specpdl-size 1000) 

;; Show column number
(setq column-number-mode t)

;; http://orgmode.org/elpa.html
(require 'package)
(add-to-list 'package-archives '("org" . "http://orgmode.org/elpa/") t)

;; Set find, grep etc.
(require 'grep)
(setq grep-program "D:/ToolBox/GnuWin32/bin/grep.exe")
(setq find-program "D:/ToolBox/GnuWin32/bin/find.exe")

;; Use xelatex instead of pdflatex since the latter does not work well
;; for Chinese characters.
(setq org-latex-pdf-process 
    '("xelatex -interaction nonstopmode -output-directory %o %f"
	  "xelatex -interaction nonstopmode -output-directory %o %f"
	  "xelatex -interaction nonstopmode -output-directory %o %f"))

;; (set-language-environment "UTF-8")
;; (set-locale-environment "UTF-8")

;; Display Chinese characters correctly in "M-! dir". 
(prefer-coding-system 'utf-8)
(setq file-name-coding-system 'gbk)
(set-terminal-coding-system 'gbk)
(set-keyboard-coding-system 'gbk)
(setq locale-coding-system 'gbk)
(set-selection-coding-system 'gbk)
(set-clipboard-coding-system 'ctext)
(set-clipboard-coding-system 'gbk)
(set-terminal-coding-system 'gbk)
(set-buffer-file-coding-system 'gbk)
(modify-coding-system-alist 'process "." 'gbk)
(setq default-process-coding-system '(gbk . gbk))

;; http://orgmode.org/worg/org-tutorials/org-publish-html-tutorial.html
(require 'ox-publish)
(setq org-publish-project-alist
  '(
    ("org-notes"
     :base-directory "D:/Workspace/Notes/org"
     :base-extension "org"
     :publishing-directory "D:/Workspace/Notes/publish"
     :recursive t
     :publishing-function org-html-publish-to-html
     :headline-levels 4  ; Just the default for this project.
     :auto-preamble t
     )  
    ("org-static"
    :base-directory "D:/Workspace/Notes/org"
    :base-extension "css\\|js\\|png\\|jpg\\|gif\\|pdf\\|mp3\\|ogg\\|swf"
    :publishing-directory "D:/Workspace/Notes/publish"
    :recursive t
    :publishing-function org-publish-attachment
    )
    ("org" :components ("org-notes" "org-static"))
   ))
  
;; Customize the latex template  
;; Still need improvements
(unless (boundp 'org-latex-classes)
  (setq org-latex-classes nil))
(require 'ox-latex)  
(add-to-list 'org-latex-classes
             '("article-zh"
               "\\documentclass[11pt]{article}\n\\usepackage{fontspec}\n\\usepackage{geometry}\n\\usepackage{hyperref}\n\\setmainfont{Microsoft Yahei}\n\\setsansfont{Microsoft Yahei}\n\\setmonofont{Microsoft Yahei}\n\\geometry{a4paper, textwidth=6.5in, textheight=10in,marginparsep=7pt, marginparwidth=.6in}\n\\author{William Zhang}\n[NO-DEFAULT-PACKAGES][NO-PACKAGES]"
               ("\\section{%s}" . "\\section*{%s}")
               ("\\subsection{%s}" . "\\subsection*{%s}")
               ("\\subsubsection{%s}" . "\\subsubsection*{%s}")
               ("\\paragraph{%s}" . "\\paragraph*{%s}")
               ("\\subparagraph{%s}" . "\\subparagraph*{%s}")))

(add-to-list 'load-path "~/.emacs.d/lisp/")
(load "graphviz-dot-mode.el" nil t t)
(add-hook 'find-file-hook (lambda()
                            (if (string= "dot" (file-name-extension
                                                buffer-file-name))
                                (progn
                                  (message "Enabling Setings for dot-mode")
                                  (setq fill-column 1000)
                                  (base-auto-pair)
                                  (local-set-key (kbd "<C-f6>") 'compile)
                                  )
                              )))

;; https://github.com/tomprince/org-mode
(load "org-export-generic.el" nil t t)

(require 'package)
(add-to-list 'package-archives
  '("melpa" . "http://melpa.milkbox.net/packages/") t)

(require 'ox-gfm)

(put 'erase-buffer 'disabled nil)

;; Keep it as the last line
;; Max the main window when emacs startup
(toggle-frame-maximized) 
