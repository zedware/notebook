;; === Debug .emacs ===
;; Turn it on: (setq debug-on-error t)
;; Turn it off: (setq debug-on-error nil)
(setq debug-on-error nil)

;; === Default encoding ===
(prefer-coding-system 'utf-8)

;; Emacs "activates" ALL installed packages BEFORE reading the 
;; user-init-file unless set package-enable-at-startup to nil 
;; in the early init file.
(setq package-enable-at-startup nil)

;; === Packages ===
;; Add the following three lines to enable package install.
;; then M-x package-install RET ox-gfm RET
;; http://orgmode.org/manual/Installation.html#Installation
;; If emacs report errors indicating some packages are not found,
;; try to find them by M-x list-packages and then install.
(require 'package)
(setq package-archives '(
   ("gnu" . "http://mirrors.tuna.tsinghua.edu.cn/elpa/gnu/")
   ("melpa" . "http://mirrors.tuna.tsinghua.edu.cn/elpa/melpa/")))
(package-initialize)

(require 'ox-gfm)

;; === Packages configurated by use-package ===
;; https://github.com/jwiegley/use-package#installing-use-package
;; This is only needed once, near the top of the file
(eval-when-compile
  ;; Following line is not needed if use-package.el is in ~/.emacs.d
  ;; (add-to-list 'load-path "<path where use-package is installed>")
  (require 'use-package))

;; Use hotkey to copy from system clipboard directly.
;; This is for Microsoft Terminal and WSL2 on Windows
;; https://github.com/spudlyo/clipetty/
(use-package clipetty
  :ensure t
  :bind ("M-w" . clipetty-kill-ring-save))

;; === Proxy ===
;; Replace proxy.google.com with your own.
;; (setq url-proxy-services '(
;;   ("no_proxy" . "^\\(localhost\\|10.*\\)")
;;   ("http" . "proxy.google.com:8080")
;;   ("https" . "proxy.google.com:8080")))

;; === Chinese font ===
;; Disable font cache to avoid freezing
(setq inhibit-compacting-font-caches t)
;; https://github.com/tumashu/cnfonts#org8dffa7c
;; M-X package-install RET cnfonts RET
(require 'cnfonts)
;; Let cnfonts enabled.
(cnfonts-enable)
;; Fix the Unicode icon in "spacemacs mode-line".
(cnfonts-set-spacemacs-fallback-fonts)

;; === Markdown ===
;; Jekyll markdown/html
(add-to-list 'auto-mode-alist '("\\.md$" . jekyll-markdown-mode))
(add-to-list 'auto-mode-alist '("\\.html" . jekyll-html-mode))

;; === Working directory ===
;; https://www.gnu.org/software/emacs/manual/html_node/elisp/System-Environment.html
(cond ((eq system-type 'windows-nt)
       (setq wdir "D:\\Workspace\\org")
       (setq pdir "D:\\Workspace\\publish"))
      ((eq system-type 'gnu/linux)
       (setq wdir "/home/wsl/workspace/org")
       (setq pdir "/home/wsl/workspace/publish"))
)
(message "Set workspace wdir as: %s" wdir)
(message "Set workspace pdir as: %s" pdir)

;; === Org ===
;; http://orgmode.org/worg/org-tutorials/org-publish-html-tutorial.html
;; Backtick and comma used to evaluate variables wdir and pdir.
(require 'ox-publish)
(setq org-publish-project-alist
  `(
    ("org-notes"
     :base-directory ,wdir
     :base-extension "org"
     :publishing-directory ,pdir
     :recursive t
     :publishing-function org-html-publish-to-html
     :headline-levels 4  ; Just the default for this project.
     :auto-preamble t
     )  
    ("org-static"
    :base-directory ,wdir
    :base-extension "css\\|js\\|png\\|jpg\\|gif\\|pdf\\|mp3\\|ogg\\|swf"
    :publishing-directory ,pdir
    :recursive t
    :publishing-function org-publish-attachment
    )
    ("org" :components ("org-notes" "org-static"))
   ))

;; https://emacs.stackexchange.com/questions/45191/how-to-place-author-and-date-under-title
(setq org-html-preamble-format 
      '(("en"
         "<h1 class=\"title\">%t</h1>
          <p class=\"subtitle\">%d By %a</p>")))

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
  "Reformat current paragraph or region to `fill-column', like `fill-paragraph' Version 2017-01-08"
  (interactive)
  ;; This command symbol has a property ¡°'compact-p¡±, the possible values are t and nil. 
  (let (($compact-p
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
(setq make-backup-files t)
(setq package-selected-packages
  (quote (ox-gfm org-plus-contrib org html2org whitespace-cleanup-mode poly-markdown jekyll-modes markdown-preview-mode html-to-markdown ox-pandoc htmlize markdown-modee)))
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
;;(setq grep-program "/usr/bin/grep")
;;(setq find-program "/usr/bin/find")
;;(setq markdown-command "/usr/local/bin/markdown")

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

(put 'upcase-region 'disabled nil)
(put 'downcase-region 'disabled nil)

;; Black background and white foreground etc.
(add-hook 'emacs-startup-hook
  (lambda ()
    (load-theme 'wheatgrass)
    ))

;; === Default directory and files === 
(message "Change working dir as: %s" wdir)
(cd wdir)
(find-file (concat (file-name-as-directory wdir) "todo.org"))
(find-file (concat (file-name-as-directory wdir) "tmp.org"))

;; END
