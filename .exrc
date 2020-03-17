set makeprg=./build.sh

set ts=4 sts=4 sw=4 expandtab

command! RunGL :Dispatch! ./rungl.sh
command! RunQt :Dispatch! ./runqt.sh

nnoremap <Leader>rg :RunGL<CR>
nnoremap <Leader>rq :RunQt<CR>

nnoremap <Leader>sc :e %:r.cpp<CR>
nnoremap <Leader>sh :e %:r.h<CR>

function! GenDecoderInstr(header)
    execute "e libmomiji/src/Decoder/" . fnameescape(a:header) . ".h"
    execute "normal! i
    \#pragma once\<CR>\<CR>
    \#include <momiji/Decoder.h>\<CR>\<CR>
    \namespace momiji::dec\<CR>
    \{\<CR>\<CR>
    \}"
    execute "w"

    execute "e libmomiji/src/Decoder/" . fnameescape(a:header) . ".cpp"
    execute "normal! i
    \#include \"" . a:header . ".h\"\<CR>\<CR>
    \#include \"./Utils.h\"\<CR>\<CR>
    \namespace momiji::dec\<CR>
    \{\<CR>\<CR>}"
    execute "w"
endfunction

function! GenCompilerInstr(header)
    execute "e libmomiji/src/Compiler/" . fnameescape(a:header) . ".h"
    execute "normal! i
    \#pragma once\<CR>\<CR>
    \#include <momiji/Compiler.h>\<CR>\<CR>
    \#include \"../Instructions/Representations.h\"\<CR>\<CR>
    \#include \"./Utils.h\"\<CR>\<CR>
    \namespace momiji::enc\<CR>
    \{\<CR>\<CR>
    \}"
    execute "w"
endfunction

function! GenExecutableInstr(header)
    execute "e libmomiji/src/Instructions/" . fnameescape(a:header) . ".h"
    execute "normal! i
    \#pragma once\<CR>\<CR>
    \#include <momiji/System.h>\<CR>\<CR>
    \namespace momiji::instr\<CR>
    \{\<CR>\<CR>
    \}"
    execute "w"

    execute "e libmomiji/src/Instructions/" . fnameescape(a:header) . ".cpp"
    execute "normal! i
    \#include \"" . a:header . ".h\"\<CR>\<CR>
    \#include \"./Utils.h\"\<CR>\<CR>
    \namespace momiji::instr\<CR>
    \{\<CR>\<CR>}"
    execute "w"
endfunction

function! GenAllInstr(header)
    call GenCompilerInstr(a:header)
    call GenDecoderInstr(a:header)
    call GenExecutableInstr(a:header)
endfunction

function! SwitchLocal()
endfunction
