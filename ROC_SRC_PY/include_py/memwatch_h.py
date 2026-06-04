# Auto-converted from memwatch.h
import typing

# memwatch.h: Header for the MEMWATCH memory leak and corruption detection library.
# memwatch.h: MEMWATCH 記憶體洩漏與損壞追蹤檢測公用程式庫的標頭檔。
__MEMWATCH_H = None
MW_ARI_NULLREAD = 0x10
MW_ARI_ABORT = 0x04
MW_ARI_RETRY = 0x02
MW_ARI_IGNORE = 0x01
MW_VAL_NEW = 0xFE
MW_VAL_DEL = 0xFD
MW_VAL_NML = 0xFC
MW_VAL_GRB = 0xFB
MW_TEST_ALL = 0xFFFF
MW_TEST_CHAIN = 0x0001
MW_TEST_ALLOC = 0x0002
MW_TEST_NML = 0x0004
MW_NML_NONE = 0
MW_NML_FREE = 1
MW_NML_ALL = 2
MW_NML_DEFAULT = 0
MW_STAT_GLOBAL = 0
MW_STAT_MODULE = 1
MW_STAT_LINE = 2
MW_STAT_DEFAULT = 0
MW_TRACE_BUFFER = 2048
MW_FREE_LIST = 64
def mwInit() -> 'None':
    """ C function: void  mwInit(void) """
    pass

def mwTerm() -> 'None':
    """ C function: void  mwTerm(void) """
    pass

def mwAbort() -> 'None':
    """ C function: void  mwAbort(void) """
    pass

def mwFlushNow() -> 'None':
    """ C function: void      mwFlushNow(void) """
    pass

def mwDoFlush(onoff: 'int') -> 'None':
    """ C function: void      mwDoFlush(int onoff) """
    pass

def mwLimit(bytes: 'int') -> 'None':
    """ C function: void      mwLimit(long bytes) """
    pass

def mwGrab(kilobytes: 'unsigned') -> 'unsigned':
    """ C function: unsigned  mwGrab(unsigned kilobytes) """
    pass

def mwDrop(kilobytes: 'unsigned') -> 'unsigned':
    """ C function: unsigned  mwDrop(unsigned kilobytes) """
    pass

def mwNoMansLand(mw_nml_level: 'int') -> 'None':
    """ C function: void      mwNoMansLand(int mw_nml_level) """
    pass

def mwStatistics(level: 'int') -> 'None':
    """ C function: void      mwStatistics(int level) """
    pass

def mwFreeBufferInfo(onoff: 'int') -> 'None':
    """ C function: void      mwFreeBufferInfo(int onoff) """
    pass

def mwAutoCheck(onoff: 'int') -> 'None':
    """ C function: void      mwAutoCheck(int onoff) """
    pass

def mwCalcCheck() -> 'None':
    """ C function: void      mwCalcCheck(void) """
    pass

def mwDumpCheck() -> 'None':
    """ C function: void      mwDumpCheck(void) """
    pass

def mwMark(p: 'None', description: 'const char', file: 'const char', line: 'unsigned') -> 'None':
    """ C function: void    * mwMark(void *p, const char *description, const char *file, unsigned line) """
    pass

def mwUnmark(p: 'None', file: 'const char', line: 'unsigned') -> 'None':
    """ C function: void    * mwUnmark(void *p, const char *file, unsigned line) """
    pass

def mwIsReadAddr(p: 'const void', len: 'unsigned') -> 'int':
    """ C function: int  mwIsReadAddr(const void *p, unsigned len) """
    pass

def mwIsSafeAddr(p: 'None', len: 'unsigned') -> 'int':
    """ C function: int  mwIsSafeAddr(void *p, unsigned len) """
    pass

def mwTest(file: 'const char', line: 'int', mw_test_flags: 'int') -> 'int':
    """ C function: int  mwTest(const char *file, int line, int mw_test_flags) """
    pass

def mwTestBuffer(file: 'const char', line: 'int', p: 'None') -> 'int':
    """ C function: int  mwTestBuffer(const char *file, int line, void *p) """
    pass

def mwAssert(arg0: 'int', arg1: 'const char', arg2: 'const char', arg3: 'int') -> 'int':
    """ C function: int  mwAssert(int, const char *, const char *, int) """
    pass

def mwVerify(arg0: 'int', arg1: 'const char', arg2: 'const char', arg3: 'int') -> 'int':
    """ C function: int  mwVerify(int, const char *, const char *, int) """
    pass

def mwTrace(format_string: 'const char', arg1: '...') -> 'None':
    """ C function: void  mwTrace(const char *format_string, ...) """
    pass

def mwPuts(text: 'const char') -> 'None':
    """ C function: void  mwPuts(const char *text) """
    pass

def mwSetAriAction(mw_ari_value: 'int') -> 'None':
    """ C function: void  mwSetAriAction(int mw_ari_value) """
    pass

def mwAriHandler(cause: 'const char') -> 'int':
    """ C function: int   mwAriHandler(const char *cause) """
    pass

def mwBreakOut(cause: 'const char') -> 'None':
    """ C function: void  mwBreakOut(const char *cause) """
    pass

def mwMalloc(arg0: 'size_t', arg1: 'const char', arg2: 'int') -> 'None':
    """ C function: void * mwMalloc(size_t, const char *, int) """
    pass

def mwMalloc_(arg0: 'size_t') -> 'None':
    """ C function: void * mwMalloc_(size_t) """
    pass

def mwRealloc(arg0: 'None', arg1: 'size_t', arg2: 'const char', arg3: 'int') -> 'None':
    """ C function: void * mwRealloc(void *, size_t, const char *, int) """
    pass

def mwRealloc_(arg0: 'None', arg1: 'size_t') -> 'None':
    """ C function: void * mwRealloc_(void *, size_t) """
    pass

def mwCalloc(arg0: 'size_t', arg1: 'size_t', arg2: 'const char', arg3: 'int') -> 'None':
    """ C function: void * mwCalloc(size_t, size_t, const char *, int) """
    pass

def mwCalloc_(arg0: 'size_t', arg1: 'size_t') -> 'None':
    """ C function: void * mwCalloc_(size_t, size_t) """
    pass

def mwFree(arg0: 'None', arg1: 'const char', arg2: 'int') -> 'None':
    """ C function: void   mwFree(void *, const char *, int) """
    pass

def mwFree_(arg0: 'None') -> 'None':
    """ C function: void   mwFree_(void *) """
    pass

def mwStrdup(arg0: 'const char', arg1: 'const char', arg2: 'int') -> 'str':
    """ C function: char * mwStrdup(const char *, const char *, int) """
    pass


# Macro function: mwASSERT(exp)
def mwASSERT(exp):
    # C implementation: while (mwAssert((int)(exp), #exp, __FILE__, __LINE__))
    pass
ASSERT = mwASSERT

# Macro function: mwVERIFY(exp)
def mwVERIFY(exp):
    # C implementation: while (mwVerify((int)(exp), #exp, __FILE__, __LINE__))
    pass
VERIFY = mwVERIFY
mwTRACE = mwTrace
TRACE = mwTRACE

# Macro function: malloc(n)
def malloc(n):
    # C implementation: mwMalloc(n, __FILE__, __LINE__)
    pass

# Macro function: strdup(p)
def strdup(p):
    # C implementation: mwStrdup(p, __FILE__, __LINE__)
    pass

# Macro function: realloc(p, n)
def realloc(p, n):
    # C implementation: mwRealloc(p, n, __FILE__, __LINE__)
    pass

# Macro function: calloc(n, m)
def calloc(n, m):
    # C implementation: mwCalloc(n, m, __FILE__, __LINE__)
    pass

# Macro function: free(p)
def free(p):
    # C implementation: mwFree(p, __FILE__, __LINE__)
    pass
CHECK = mwTest(__FILE__, __LINE__, MW_TEST_ALL)

# Macro function: CHECK_THIS(n)
def CHECK_THIS(n):
    # C implementation: mwTest(__FILE__, __LINE__, n)
    pass

# Macro function: CHECK_BUFFER(b)
def CHECK_BUFFER(b):
    # C implementation: mwTestBuffer(__FILE__, __LINE__, b)
    pass

# Macro function: MARK(p)
def MARK(p):
    # C implementation: mwMark(p, #p, __FILE__, __LINE__)
    pass

# Macro function: UNMARK(p)
def UNMARK(p):
    # C implementation: mwUnmark(p, __FILE__, __LINE__)
    pass

# Macro function: mwASSERT(exp)
def mwASSERT(exp):
    # C implementation: 
    pass
ASSERT = mwASSERT

# Macro function: mwVERIFY(exp)
def mwVERIFY(exp):
    # C implementation: exp
    pass
VERIFY = mwVERIFY
mwTRACE = (0 if 1 else mwDummyTraceFunction)
TRACE = mwTRACE
def mwDummyTraceFunction(arg0: 'const char', arg1: '...') -> 'None':
    """ C function: void  mwDummyTraceFunction(const char *, ...) """
    pass


# Macro function: mwDoFlush(n)
def mwDoFlush(n):
    # C implementation: 
    pass

# Macro function: mwPuts(s)
def mwPuts(s):
    # C implementation: 
    pass
mwInit = None

# Macro function: mwGrab(n)
def mwGrab(n):
    # C implementation: 
    pass

# Macro function: mwDrop(n)
def mwDrop(n):
    # C implementation: 
    pass

# Macro function: mwLimit(n)
def mwLimit(n):
    # C implementation: 
    pass

# Macro function: mwTest(f, l)
def mwTest(f, l):
    # C implementation: 
    pass

# Macro function: mwSetOutFunc(f)
def mwSetOutFunc(f):
    # C implementation: 
    pass

# Macro function: mwSetAriFunc(f)
def mwSetAriFunc(f):
    # C implementation: 
    pass
mwDefaultAri = None
mwNomansland = None

# Macro function: mwStatistics(f)
def mwStatistics(f):
    # C implementation: 
    pass

# Macro function: mwMark(p, t, f, n)
def mwMark(p, t, f, n):
    # C implementation: (p)
    pass

# Macro function: mwUnmark(p, f, n)
def mwUnmark(p, f, n):
    # C implementation: (p)
    pass

# Macro function: mwMalloc(n, f, l)
def mwMalloc(n, f, l):
    # C implementation: malloc(n)
    pass

# Macro function: mwStrdup(p, f, l)
def mwStrdup(p, f, l):
    # C implementation: strdup(p)
    pass

# Macro function: mwRealloc(p, n, f, l)
def mwRealloc(p, n, f, l):
    # C implementation: realloc(p, n)
    pass

# Macro function: mwCalloc(n, m, f, l)
def mwCalloc(n, m, f, l):
    # C implementation: calloc(n, m)
    pass

# Macro function: mwFree(p)
def mwFree(p):
    # C implementation: free(p)
    pass

# Macro function: mwMalloc_(n)
def mwMalloc_(n):
    # C implementation: malloc(n)
    pass

# Macro function: mwRealloc_(p, n)
def mwRealloc_(p, n):
    # C implementation: realloc(p, n)
    pass

# Macro function: mwCalloc_(n, m)
def mwCalloc_(n, m):
    # C implementation: calloc(n, m)
    pass

# Macro function: mwFree_(p)
def mwFree_(p):
    # C implementation: free(p)
    pass

# Macro function: mwAssert(e, es, f, l)
def mwAssert(e, es, f, l):
    # C implementation: 
    pass

# Macro function: mwVerify(e, es, f, l)
def mwVerify(e, es, f, l):
    # C implementation: (e)
    pass
mwTrace = mwDummyTrace

# Macro function: mwTestBuffer(f, l, b)
def mwTestBuffer(f, l, b):
    # C implementation: (0)
    pass
CHECK = None

# Macro function: CHECK_THIS(n)
def CHECK_THIS(n):
    # C implementation: 
    pass

# Macro function: CHECK_BUFFER(b)
def CHECK_BUFFER(b):
    # C implementation: 
    pass

# Macro function: MARK(p)
def MARK(p):
    # C implementation: (p)
    pass

# Macro function: UNMARK(p)
def UNMARK(p):
    # C implementation: (p)
    pass
mwNCur: 'int' = 0
mwNLine: 'int' = 0
def h() -> 'MemWatc':
    """ C function: MemWatc h() """
    pass

mwNew = new (__FILE__, __LINE__)
mwDelete = "(mwNCur = 1, mwNFile = __FILE__, mwNLine = __LINE__), delete"