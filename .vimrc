"=Different kinds of vim scripts, including ctags, cscope, etc.
" https://github.com/vim-scripts
" http://cscope.sourceforge.net/cscope_vim_tutorial.html
" https://github.com/vim-scripts/a.vim

"=Encoding
set encoding=utf-8

"=Vundle & YCM etc.
" http://unixnme.blogspot.com/2017/03/how-to-install-youcompleteme-vim-plugin.html
" https://github.com/VundleVim/Vundle.vim#about
set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'

Plugin 'Valloric/YouCompleteMe'

Plugin 'tpope/vim-fugitive'

" The following are examples of different formats supported.
" Keep Plugin commands between vundle#begin/end.
" plugin on GitHub repo
" Plugin 'tpope/vim-fugitive'
" plugin from http://vim-scripts.org/vim/scripts.html
" Plugin 'L9'
" Git plugin not hosted on GitHub
" Plugin 'git://git.wincent.com/command-t.git'
" git repos on your local machine (i.e. when working on your own plugin)
" Plugin 'file:///home/gmarik/path/to/plugin'
" The sparkup vim script is in a subdirectory of this repo called vim.
" Pass the path to set the runtimepath properly.
Plugin 'rstacruz/sparkup', {'rtp': 'vim/'}
" Install L9 and avoid a Naming conflict if you've already installed a
" different version somewhere else.
" Plugin 'ascenator/L9', {'name': 'newL9'}

" All of your Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required
" To ignore plugin indent changes, instead use:
"filetype plugin on
"
" Brief help
" :PluginList       - lists configured plugins
" :PluginInstall    - installs plugins; append `!` to update or just :PluginUpdate
" :PluginSearch foo - searches for foo; append `!` to refresh local cache
" :PluginClean      - confirms removal of unused plugins; append `!` to auto-approve removal
"
" see :h vundle for more details or wiki for FAQ
" Put your non-Plugin stuff after this line
" let g:ycm_global_ycm_extra_conf = '~/.vim/bundle/YouCompleteMe/.ycm_extra_conf.py'

"=Coding style and syntax
set nu
set ts=2
set sw=2
set expandtab
set hlsearch
set path+=**
set cc=72
syntax on

" Even use 2 for Python
autocmd FileType python setlocal shiftwidth=2 tabstop=2

" Spell check for git commit
autocmd Filetype gitcommit setlocal spell " textwidth=72

"=Color scheme
colorscheme koehler

"=ctags
let Tlist_Ctags_Cmd = "/usr/bin/ctags"
map <F4> :TlistToggle<cr>
map <F8> :!/usr/bin/ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .<CR>

"=hightlight
nnoremap <C-K> :call HighlightNearCursor()<CR>
function HighlightNearCursor()
  if ! exists("s:highlightcursor" )
    match Todo /\k*\%#\k*/
    let s:highlightcursor= 1
  else
    match None
    unlet s:highlightcursor
  endif
endfunction
