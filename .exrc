set makeprg=./build.sh

command! RunGL :Dispatch! ./rungl.sh
command! RunQt :Dispatch! ./runqt.sh

nnoremap <Leader>rg :RunGL<CR>
nnoremap <Leader>rq :RunQt<CR>
