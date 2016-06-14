#include "value.h"
#include "class.h"
#include "static.h"
#include "console.h"

#include "c_array.h"
#include "c_numeric.h"



//================================================================
/*!@brief
  find class from a object

  @param  vm
  @param  obj
  @return pointer to mrb_class
*/
mrb_class *find_class_by_object(struct VM *vm, mrb_object *obj)
{
  mrb_class *cls = static_class_object;
  switch( obj->tt ){
    case MRB_TT_ARRAY:
      cls = static_class_array;
      break;
    case MRB_TT_FIXNUM:
      cls = static_class_fixnum;
      break;
    case MRB_TT_FALSE:
      cls = static_class_false;
      break;
    case MRB_TT_TRUE:
      cls = static_class_true;
      break;
#if MRUBYC_USE_FLOAT
    case MRB_TT_FLOAT:
      cls = static_class_float;
      break;
#endif
    default:
      break;
  }
  return cls;
}



//================================================================
/*!@brief
  find method from

  @param  vm
  @param  recv
  @param  sym_id
  @return
*/
mrb_proc *find_method(struct VM *vm, mrb_value recv, mrb_sym sym_id)
{
  mrb_class *cls = find_class_by_object(vm, &recv);

  while( cls != 0 ) {
    mrb_proc *proc = cls->procs;
    while( proc != 0 ) {
      if( proc->sym_id == sym_id ) {
        return proc;
      }
      proc = proc->next;
    }
    cls = cls->super;
  }
  return 0;
}



void mrb_define_method(mrb_class *cls, const char *name, mrb_func_t cfunc)
{
  mrb_proc *rproc = mrb_rproc_alloc(name);
  rproc->c_func = 1;  // c-func
  rproc->next = cls->procs;
  cls->procs = rproc;
  rproc->func.func = cfunc;
}




void mrb_define_method_proc(mrb_class *cls, mrb_sym sym_id, mrb_proc *rproc)
{
  rproc->c_func = 0;
  rproc->sym_id = sym_id;
  rproc->next = cls->procs;
  cls->procs = rproc;
}



// Object - puts
void c_puts(mrb_vm *vm, mrb_value *v)
{
  mrb_object *arg0 = v+1;
  switch( arg0->tt ){
  case MRB_TT_EMPTY:
    console_printf("(empty)");
    break;
  case MRB_TT_TRUE:
    console_printf("true");
    break;
  case MRB_TT_FALSE:
    console_printf("");
    break;
  case MRB_TT_NIL:
    console_printf("");
    break;
  case MRB_TT_FIXNUM:
    console_printf("%d", arg0->value.i);
    break;
  case MRB_TT_FLOAT:
#if MRUBYC_USE_FLOAT
    console_printf("%f", arg0->value.d);
#else
    console_printf("(float)")
#endif
    break;
  case MRB_TT_OBJECT:
    console_printf("");
    break;
  case MRB_TT_CLASS:
    console_printf("(class)");
    break;
  MRB_TT_PROC,
    console_printf("(proc)");
    break;
  MRB_TT_ARRAY,
    console_printf("(array)");
    break;
  default:
    console_printf("Not supported: MRB_TT_XX(%d)", arg0->tt);
    break;
  }
  console_printf("\n");
}

// Object - add
void c_add(mrb_vm *vm, mrb_value *v)
{
  SET_INT_RETURN( GET_INT_ARG(0) + GET_INT_ARG(1) );
}



static void mrb_init_class_object(void)
{
  // Class
  static_class_object = mrb_class_alloc("Object", 0);
  // Methods
  mrb_define_method(static_class_object, "puts", c_puts);
  mrb_define_method(static_class_object, "add", c_add);

}


// =============== FalseClass

void c_false_ne(mrb_vm *vm, mrb_value *v)
{
  mrb_object *arg0 = v+1;
  if( arg0->tt == MRB_TT_FALSE ){
    SET_FALSE_RETURN();
  } else {
    SET_TRUE_RETURN();
  }
}


static void mrb_init_class_false(void)
{
  // Class
  static_class_false = mrb_class_alloc("FalseClass", static_class_object);
  // Methods
  mrb_define_method(static_class_false, "!=", c_false_ne);

}


// =============== TrueClass

void c_true_ne(mrb_vm *vm, mrb_value *v)
{
  mrb_object *arg0 = v+1;
  if( arg0->tt == MRB_TT_TRUE ){
    SET_FALSE_RETURN();
  } else {
    SET_TRUE_RETURN();
  }
}


static void mrb_init_class_true(void)
{
  // Class
  static_class_true = mrb_class_alloc("TrueClass", static_class_object);
  // Methods
  mrb_define_method(static_class_true, "!=", c_true_ne);

}


void mrb_init_class(void)
{
  mrb_init_class_object();
  mrb_init_class_false();
  mrb_init_class_true();

  mrb_init_class_fixnum();
#if MRUBYC_USE_FLOAT
  mrb_init_class_float();
#endif

  mrb_init_class_array();
}
