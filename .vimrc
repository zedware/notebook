" ctags, cscope etc.
" https://github.com/vim-scripts
" http://cscope.sourceforge.net/cscope_vim_tutorial.html
" https://github.com/vim-scripts/a.vim
set nu
set ts=2
set sw=2
set expandtab
set paste
set hlsearch
set path+=**

syntax on

" colorscheme murphy

" Even use 2 for Python
autocmd FileType python setlocal shiftwidth=2 tabstop=2

" Spell check for git commit
autocmd Filetype gitcommit setlocal spell " textwidth=72

" ctags
let Tlist_Ctags_Cmd = "/usr/bin/ctags"
map <F4> :TlistToggle<cr>
map <F8> :!/usr/bin/ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .<CR>

"
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

