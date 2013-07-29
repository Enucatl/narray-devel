static void
<%=c_iterator%>(na_loop_t *const lp)
{
    size_t  i;
    char    *p1, *p2, *p3;
    ssize_t s1, s2, s3;
    size_t  *idx1, *idx2, *idx3;
    dtype    x, y;
    INIT_COUNTER(lp, i);
    INIT_PTR(lp, 0, p1, s1, idx1);
    INIT_PTR(lp, 1, p2, s2, idx2);
    INIT_PTR(lp, 2, p3, s3, idx3);
    if (idx1||idx2||idx3) {
        for (; i--;) {
            LOAD_DATA_STEP(p1, s1, idx1, dtype, x);
            LOAD_DATA_STEP(p2, s2, idx2, dtype, y);
            x = m_pow(x,y);
            STORE_DATA_STEP(p3, s3, idx3, dtype, x);
        }
    } else {
        for (; i--;) {
            x = *(dtype*)p1;
            p1+=s1;
            y = *(dtype*)p2;
            p2+=s2;
            x = m_pow(x,y);
            *(dtype*)p3 = x;
            p3+=s3;
        }
    }
}

static void
<%=c_iterator%>_int32(na_loop_t *const lp)
{
    size_t  i;
    char   *p1, *p2, *p3;
    ssize_t s1, s2, s3;
    size_t  e2;
    size_t *idx1, *idx2, *idx3;
    dtype    x;
    int32_t y;
    INIT_COUNTER(lp, i);
    INIT_PTR(lp, 0, p1, s1, idx1);
    INIT_PTR_ELM(lp, 1, p2, s2, idx2, e2);
    INIT_PTR(lp, 2, p3, s3, idx3);
    if (idx1||idx2||idx3) {
        for (; i--;) {
            LOAD_DATA_STEP(p1, s1, idx1, dtype, x);
            LOAD_INT_STEP(p2, s2, idx2, e2, int32_t, y);
            x = m_pow_int(x,y);
            STORE_DATA_STEP(p3, s3, idx3, dtype, x);
        }
    } else {
        for (; i--;) {
            x = *(dtype*)p1;
            p1+=s1;
            LOAD_INT(p2,e2,y);
            p2+=s2;
            x = m_pow_int(x,y);
            *(dtype*)p3 = x;
            p3+=s3;
        }
    }
}

static VALUE
<%=c_instance_method%>_self(VALUE self, VALUE other)
{
    ndfunc_arg_in_t ain[2] = {{cT,0},{cT,0}};
    ndfunc_arg_in_t ain_i[2] = {{cT,0},{cInt32,0}};
    ndfunc_arg_out_t aout[1] = {{cT,0}};
    ndfunc_t ndf = { <%=c_iterator%>, STRIDE_LOOP, 2, 1, ain, aout };
    ndfunc_t ndf_i = { <%=c_iterator%>_int32, STRIDE_LOOP, 2, 1, ain_i, aout };

    // fixme : use na.integer?
    if (FIXNUM_P(other) || rb_obj_is_kind_of(other,cInt32)) {
        return na_ndloop(&ndf_i, 2, self, other);
    } else {
        return na_ndloop(&ndf, 2, self, other);
    }
}

/*
  <%=op%>.
  @overload <%=op_map%> other
  @param [NArray,Numeric] other
  @return [NArray] self <%=op%> other.
*/
static VALUE
<%=c_instance_method%>(VALUE self, VALUE other)
{
    VALUE klass, v;
    klass = na_upcast(CLASS_OF(self),CLASS_OF(other));
    if (klass==cT) {
        return <%=c_instance_method%>_self(self,other);
    } else {
        v = rb_funcall(klass, id_cast, 1, self);
        return rb_funcall(v, id_pow, 1, other);
    }
}