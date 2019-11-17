define print_threads
    set $p = (class base_ilist)threads
    set $first = $p->first
    set $last = &($p->last)
    printf "first=%p, last=%p\n", first, last
    while first != last
        set $first = $first->next
    end
end

document print_threads
Print the `threads` list
Syntax: print_threads
end

