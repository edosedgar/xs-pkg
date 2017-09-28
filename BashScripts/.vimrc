set nocompatible
filetype off

set rtp+=~/.vim/bundle/vundle/
call vundle#begin()

"call vundle#rc()

Plugin 'tpope/vim-fugitive'
Bundle 'L9'
Bundle 'FuzzyFinder'
"Bundle 'rails.vim'
"Bundle 'vim-swift'
Plugin 'bling/vim-airline'
Plugin 'vim-airline/vim-airline-themes'
Bundle 'erickzanardo/vim-xclip'
"Plugin 'altercation/vim-colors-solarized'
Plugin 'SyntaxAttr.vim'

call vundle#end()

"filetype plugin indent on
:set smartindent
:match ErrorMsg '\s\+$'
:set list
:set listchars=tab:>-
:set number
:colorscheme delek
:filetype plugin on
:set tabstop=8
:set shiftwidth=8
":set expandtab "
:syntax on
:set mouse=a
autocmd FileType c set omnifunc=ccomplete#Complete
"let g:airline#extensions#tagbar#enabled = 1
set clipboard=unnamedplus
vmap <C-c> "+y

let g:molokai_original = 1
let g:rehash256 = 1
colorscheme molokai
if &term =~ '256color'
    " Disable Background Color Erase (BCE) so that color schemes
    " work properly when Vim is used inside tmux and GNU screen.
    set t_ut=
endif

let g:airline_theme='badwolf'
set wildmenu "visual command autocomplete"
set wildmode=longest,list,full

:set laststatus=2
:set encoding=utf-8
:set t_Co=256

let g:airline_powerline_fonts = 1
if !exists('g:airline_symbols')
  let g:airline_symbols = {}
endif

set wrap
set linebreak
" note trailing space at end of next line
set showbreak=>\ \ \
set list
set listchars+=trail:◦
