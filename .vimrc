" VIM Configuration File
" Description: Optimized for C/C++ development, but useful also for other things
" Author: Gerhard Gappmeier
"

" set UTF-8 encoding
set enc=utf-8
set fenc=utf-8
set termencoding=utf-8
"set font and size
set guifont=Andale\ Mono\ 12
" disable vi compatibility (emulation of old bugs)
set nocompatible
" use indentation of previous line
set autoindent
" use intelligent indentation for C
set smartindent
" configure tabwidth and insert spaces instead of tabs

set tabstop=2        " tab width is 4 spaces
set shiftwidth=2     " indent also with 4 spaces
set expandtab        " expand tabs to spaces

" makefiles must use tabs
autocmd FileType * set expandtab
autocmd BufRead,BufNewFile [Mm]akefile set noexpandtab
autocmd BufRead,BufNewFile *.py set noexpandtab
autocmd FileType make set noexpandtab
autocmd FileType python set noexpandtab

" set textwidth to wrap at 80 characters
"set textwidth=80

" turn syntax highlighting on
syntax on

set t_Co=256
colorscheme wombat256
"colorscheme desert256
"colorscheme xoria256
" turn line numbers on
set number
" highlight matching braces
set showmatch
" intelligent comments
set comments=sl:/*,mb:\ *,elx:\ */
" set mouse mode on
set mouse=a

set tags+=~/.vim/systags

" begin highlighting red after 80 characters
" highlight OverLength ctermbg=red ctermfg=white guibg=#592929
" match OverLength /\%81v.\+/

"folding settings
set foldmethod=indent   "fold based on indent
set foldnestmax=10      "deepest fold is 10 levels
set nofoldenable        "dont fold by default
set foldlevel=1         "this is just what i use

" disable visual bell and audio bell
" Enhanced keyboard mappings
"
" in normal mode F2 will save the file
nmap <F2> :w<CR>
" in insert mode F2 will exit insert, save, enters insert again
imap <F2> <ESC>:w<CR>i
" switch between header/source with F4
map <F4> :e %:p:s,.h$,.X123X,:s,.cpp$,.h,:s,.X123X$,.cpp,<CR>
" recreate tags file with F5
map <F5> :!ctags -R -c++-kinds=+p -fields=+iaS -extra=+q .<CR>
" build using makeprg with <F7>
map <F7> :make<CR>
" build using makeprg with <S-F7>
map <S-F7> :make clean all<CR>
" goto definition with F12
map <F12> <C-]>
  " spell settings
"  :setlocal spell spelllang=en
  " set the spellfile - folders must exist
"  set spellfile=~/.vim/spellfile.add
"  map <M-Down> ]s
"  map <M-Up> [s
"endif

" turn off wrapping
set nowrap

" turn off screen flashing (it's really really annoying)
set novisualbell

