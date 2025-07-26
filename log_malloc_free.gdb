set pagination off

# Redirect all output to a log file
set logging file malloc_free_log.txt
set logging overwrite on
set logging on

# ----- malloc -----
break malloc
commands
    silent
    printf "[MALLOC] size=%zu\n", $rdi
    continue
end

break malloc return
commands
    silent
    printf "[MALLOC RETURN] ptr=%p\n", $rax
    continue
end

# ----- calloc -----
break calloc
commands
    silent
    printf "[CALLOC] nmemb=%zu size=%zu\n", $rdi, $rsi
    continue
end

break calloc return
commands
    silent
    printf "[CALLOC RETURN] ptr=%p\n", $rax
    continue
end

# ----- realloc -----
break realloc
commands
    silent
    printf "[REALLOC] ptr=%p size=%zu\n", $rdi, $rsi
    continue
end

break realloc return
commands
    silent
    printf "[REALLOC RETURN] new_ptr=%p\n", $rax
    continue
end

# ----- free -----
break free
commands
    silent
    printf "[FREE] ptr=%p\n", $rdi
    continue
end
