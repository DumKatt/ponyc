class List[A:Any{p}] is Iterable[A]
  var item:A
  var next:(List[A]{var}|None)

  fun default(item:A, next:(List[A]{var}|None) = None):List[A]{iso} =
    \List[A](item->item, next->next).absorb()

  fun default(item:A{val}, next:(List[A]{val}|None) = None):List[A]{val} =
    \List[A](item->item, next->next).absorb()

  fun{iso|var|val} item():A{p->this} = item

  fun{iso|var} setitem(a:A):A = swap(a, item)

  fun{iso|var|val} next():(List[A]{var->this}|None) = next

  fun{iso|var} setnext(a:(List[A]{var}|None) = None) = swap(a, next)

  fun{var|val} iterator():ListIterator[A]{var} = ListIterator(this)

class ListIterator[A:Any{p}] is Iterator[A]
  var list:List[A]{var|val}

  fun default(list:List[A]{var|val}) =
    \ListIterator(list->list).absorb()

  fun{iso|var|val} has_next():Bool =
    match list.next()
      | None = false
      | = true
      end

  fun{iso|var} next():A{p->this} =
    var item = list.get();
    list = match list.get_next()
      | as a:List[A]{var|val} = a
      | None = list
      end;
    item

/*
vector
array
single list
double list
queue
stack
double-ended queue
tree
heap
bag
set
map
*/
