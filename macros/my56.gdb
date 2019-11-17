### Comments
### 1. ### denotes a section of macros used together.
### 2. #   denotes a single macro.
### 3. the scope of the $ variables are global, so take great care to the
###    variable names used in the macros in a section.  The values in the
###    caller may be overide by the callee!

### trx list dump

#
define print_a_lock
    set $p = (struct ib_lock_t*)$arg0
    if $p != 0
        printf "trx: %p, type_mode=%d\n", $p->trx, $p->type_mode
        # Naive way to dump the contents.
        # p *$p
    end
end

document print_a_lock
Print a lock object
Syntax: print_a_lock <lock>
Sample: print_a_lock 0x12345678
end

#
define print_a_lock_list
    set $p = (struct ut_list_base<ib_lock_t>)$arg0
    set $n = (char*)$arg1
    printf "%s: count=%d, start=%p, end=%p\n", \
        $n, $p->count, $p->start, $p->end
    if $p->count != 0
        # $x's type is struct ib_lock_t *.
        set $x_a = $p->start
        while $x_a != 0
            printf "lock: %p\n", $x_a
            print_a_lock $x_a
            set $x_a = $x_a->trx_locks->next
        end
    end
end

document print_a_lock_list
Print a lock list
Syntax: print_a_lock_list <lock_list> <name>
Sample: print_a_lock_list trx_sys->rw_trx_list->start->lock->trx_locks "x"
end

#
define print_a_trx_list
    set $p = (struct ut_list_base<trx_t>)$arg0
    set $n = (char*)$arg1
    printf "%s: count=%d, start=%p, end=%p\n", \
        $n, $p->count, $p->start, $p->end
    if $p->count != 0
        set $x = $p->start
        while $x != 0
            printf "trx: %p\n", $x
            print_a_lock_list $x->lock->trx_locks "N/A"
            set $x = $x->trx_list->next
        end
    end
end

document print_a_trx_list
Print a single trx_list
Syntax: print_a_trx_list <trx_list> <list_name>
Sample: print_a_trx_list trx_sys->rw_trx_list "rw"
end

#
define print_trx_list
    print_a_trx_list trx_sys->mysql_trx_list "mysql"
    printf "\n"
    print_a_trx_list trx_sys->rw_trx_list "rw"
    printf "\n"
    print_a_trx_list trx_sys->ro_trx_list "ro"
end

document print_trx_list
Print the trx_list(s) in trx_sys
Syntax: print_trx_list
end

### query cache dump

define print_query_cache_block_listp
    set $p = (Query_cache_block*)$arg0
    printf "node: 0x%x\n", $p
    if $p != 0
        set $x = (Query_cache_block*)($p->pnext)
        while $x != 0 && $x != $p
            printf "node: 0x%x\n", $x->pnext
            set $x = (Query_cache_block*)($x->pnext)
        end
    end
end

document print_query_cache_block_listp
Prints the list by pnext.
Syntax: print_query_cache_block_listp <addr>
end

define print_query_cache_block_list
    set $p = (Query_cache_block*)$arg0
    printf "node: 0x%x\n", $p
    if $p != 0
        set $x = (Query_cache_block*)($p->next)
        while $x != 0 && $x != $p
            printf "node: 0x%x\n", $x->next
            set $x = (Query_cache_block*)($x->next)
        end
    end
end

document print_query_cache_block_list
Prints the list by next
Syntax: print_query_cache_block_list <addr>
end

define query_cache_dump
    set $query_cache = (Query_cache*)$arg0
    set $i = (Query_cache_block*)($query_cache->first_block)
    printf "     length      used type n_tables i next prev pnext pprev\n"
    while true
        printf "%10lu %10lu %1d %2d 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx\n", \
          $i->length, $i->used, (int)($i->type),    \
          $i->n_tables, $i,   \
          $i->next, $i->prev, $i->pnext,    \
          $i->pprev
        set $i = $i->pnext
        if $i == $query_cache->first_block
            break;  
    end
end

document query_cache_dump
Dump the query cache.
Syntax: query_cache_dump <query_cache>
end
