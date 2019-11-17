### Comments
### 1. ### denotes a section of macros used together.
### 2. #   denotes a single macro.
### 3. the scope of the $ variables are global, so take great care to the
###    variable names used in the macros in a section.  The values in the
###    caller may be overide by the callee!

### trx list dump

#
define walk_trx
  set $p = trx_sys->trx_list->start
  printf "p=%p\n", $p
  walk_trx_locks $p
  while $p != 0
      set $p = $p->trx_list->next
      printf "p=%p\n", $p
      walk_trx_locks $p
  end
end

document walk_trx
Print trx.
Syntax: walk_trx
Sample: walk_trx
end

#
define walk_trx_locks
    if $arg0 != 0
        set $q = ((trx_t *)$arg0)->trx_locks
        if $q->count > 0
            set $q = $q->start    
            if $q != 0
                printf "q=%p, type_mode=%d\n", $q, $q->type_mode
                print_a_lock $q
                while $q != 0
                    set $q = $q->trx_locks->next
                    if $q != 0
                        printf "q=%p, type_mode=%d\n", $q, $q->type_mode
                        print_a_lock $q
                    end
                end
            end
        end 
    end
end

#
define print_a_lock
    set $lock = $arg0

    # lock modes
    set $LOCK_IS = 0
    set $LOCK_IX = 1
    set $LOCK_S = 2
    set $LOCK_X = 3
    set $LOCK_AUTO_INC = 4
    set $LOCK_NONE = 5
    set $LOCK_NUM = $LOCK_NONE

    set $LOCK_TABLE = 16
    set $LOCK_REC = 32

    set $LOCK_WAIT = 256

    set $LOCK_ORDINARY = 0
    set $LOCK_GAP = 512
    set $LOCK_REC_NOT_GAP = 1024
    set $LOCK_INSERT_INTENTION = 2048

    #  438   return(1 & ((const byte*) &lock[1])[byte_index] >> bit_index);
    if $lock != 0
        if $lock->type_mode & $LOCK_REC
            printf "REC\n"
            printf "space=%d, page_no=%d, n_bits=%d\n", $lock->un_member->rec_lock->space, \
                                                        $lock->un_member->rec_lock->page_no, \
                                                        $lock->un_member->rec_lock->n_bits
            set $low = 0
            set $high = $lock->un_member->rec_lock->n_bits / 8
            set $remain = $lock->un_member->rec_lock->n_bits % 8
            set $bits = (char *)&($lock[1])
            while $low != $high
                printf "bits[%d]=%x ", $low, *($bits + $low)
                set $low = $low + 1
            end
            if $remain != 0
                printf "bits[%d]=%x ", $low, *($bits + $low)
            end
            printf "\n"
        else 
        if $lock->type_mode & $LOCK_TABLE
            printf "TABLE\n"
        end
    end
end

