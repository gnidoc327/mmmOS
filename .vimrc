set number
set ai
set si
set tabstop=4
set ignorecase
set hlsearch
set background=dark
set nocompatible
set fileencodings=utf-8,euc-kr
set bs=indent,eol,start
set history=1000
set title
set showmatch
set wmnu
set cindent
set autoindent
set smartindent
set ruler
set sw=1

au BufReadPost *
\ if line("'\"") > 0 && line("'\"") <= line("$") |
\ exe "norm g'\"" |
\ endif

if has("syntax")
	syntax on
endif

map <F4> :Tlist<cr>
filetype plugin on

"colorscheme ???
