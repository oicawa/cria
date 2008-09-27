#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "_Memory.h"

static void default_error_handler(
    Memory  controller,
    char    *filename, 
    int     line, 
    char    *msg
);



//メモリ管理モジュールの実体。
//デザインパターン的にはシングルトンになる。
static struct MemoryTag st_default_controller = {
    NULL,                   //stderr
    default_error_handler,
    MEMORY_FAIL_MODE_EXIT
};



Memory mem_default_controller = &st_default_controller;



typedef union {
    long        l_dummy;
    double      d_dummy;
    void        *p_dummy;
} Align;



#define MARK_SIZE       (4)



typedef struct {
    int             size;
    char            *filename;
    int             line;
    Header          *prev;
    Header          *next;
    unsigned char   mark[MARK_SIZE];
} HeaderStruct;



#define ALIGN_SIZE      (sizeof(Align))
#define revalue_up_align(val)   ((val) ? (((val) - 1) / ALIGN_SIZE + 1) : 0)
#define HEADER_ALIGN_SIZE       (revalue_up_align(sizeof(HeaderStruct)))
#define MARK (0xCD)



union HeaderTag {
    HeaderStruct        s;
    Align               u[HEADER_ALIGN_SIZE];
};


typedef union {
    long        l_dummy;
    double      d_dummy;
    void        *p_dummy;
} Cell;

#define CELL_SIZE               (sizeof(Cell))
#define DEFAULT_PAGE_SIZE       (1024)  /* cell num */

typedef struct MemoryPage_tag MemoryPage;
typedef MemoryPage *MemoryPageList;

struct MemoryPage_tag {
    int                 cell_num;
    int                 use_cell_num;
    MemoryPageList      next;
    Cell                cell[1];
};

struct MemoryStorageTag {
    MemoryPageList      page_list;
    int                 current_page_size;
};

#define larger(a, b) (((a) > (b)) ? (a) : (b))

static void
default_error_handler(
    Memory  controller,
    char    *filename,
    int     line, 
    char    *msg
)
{
    fprintf(controller->error_fp, "Memory:%s failed in %s at %d\n", msg, filename, line);
}



static void
error_handler(
    Memory  controller,
    char    *filename,
    int     line,
    char    *msg
)
{
    if (controller->error_fp == NULL)
        controller->error_fp = stderr;

    
    controller->error_handler(controller, filename, line, msg);


    if (controller->fail_mode == MEMORY_FAIL_MODE_EXIT)
        exit(1);
}



//メモリ管理コントローラモジュールを生成。
//デザインパターン的には
Memory
Memory_new(
    void
)
{
    Memory  p;

    p = Memory_mallocFunction(&st_default_controller, __FILE__, __LINE__, sizeof(struct MemoryTag));
    *p = st_default_controller;

    return p;
}



//全メモリの解放
void
Memory_dispose(
    void
)
{
    while(mem_default_controller->block_header != NULL)
    {
        void* ptr = (char*)mem_default_controller->block_header + sizeof(Header);
        Memory_freeFunction(mem_default_controller, ptr);
    }
}
    


//メモリ管理コントローラで、今まで確保してきたメモリ（インスタンス）を
//各メモリ領域共通で持っている「Header」の「PrevHeader」と「NextHeader」を使って、
//紐付けをおこなう。スタックのようなイメージ？
static void
chain_block(
    Memory  controller,
    Header  *new_header
)
{
    //メモリ管理コントローラに「BlockHeader」が存在した場合、
    //該当Headerの「PrevHeader」に新規Headerを登録。
    if (controller->block_header)
        controller->block_header->s.prev = new_header;


    //新規Headerの「PrevHeaderはNULL。
    //「NextHeader」には、メモリ管理コントローラのBlockHeaderを登録。
    new_header->s.prev = NULL;
    new_header->s.next = controller->block_header;

    
    //最後に、メモリ管理コントローラの「BlockHeader」に
    //新規Headerへのポインタにすげ替え。
    controller->block_header = new_header;
}



//いまいち何をしたいのか分からない。。
//前半のelseブロック意外、実質的に不要な処理を行っていないか？
static void
rechain_block(
    Memory  controller,
    Header  *header
)
{
    //渡されてきたHeaderの「PrevHeader」が存在した場合は
    //「PrevHeader」の「NextHeader」に渡されてきたHeaderを登録
    if (header->s.prev)
        header->s.prev->s.next = header;
    //渡されてきたHeaderの「PrevHeader」が存在しなかった場合は
    //メモリ管理コントローラのBlockHeaderに渡されてきたHeaderを登録。
    else
        controller->block_header = header;


    //渡されてきたHeaderの「NextHeader」が存在した場合、
    //「NextHeader」の「PrevHeader」に、自分自身を登録
    if (header->s.next)
        header->s.next->s.prev = header;
}



//渡されたHeaderを削除し、前後のHeaderを連結する。
static void
unchain_block(
    Memory  controller,
    Header  *header
)
{
    //渡されてきたHeaderの「PrevHeader」が存在した場合、
    //「PrevHader」の「NextHeader」に、自身の「NextHeader」を登録
    if (header->s.prev)
        header->s.prev->s.next = header->s.next;
    //渡されてきたHeaderの「PrevHeader」が存在しなかった場合、
    //メモリ管理コントローラのBlockHeaderに、自身の「NextHeader」を登録
    else
        controller->block_header = header->s.next;


    //渡されてきたHeaderの「NextHeader」が存在した場合、
    //「NextHeader」の「PrevHeader」に自身の「PrevHeader」を登録
    if (header->s.next)
        header->s.next->s.prev = header->s.prev;

}



//確保した領域のヘッダ部分に確保時の情報をセットする。
void
set_header(
    Header  *header,
    int     size,
    char    *filename,
    int     line
)
{
    header->s.size = size;
    header->s.filename = filename;
    header->s.line = line;
    memset(header->s.mark, MARK, (char*)&header[1] - (char*)header->s.mark);
}



//確保した領域のフッタ部分を初期化する。（確保領域の末尾4バイト分を0xCDで初期化）
void
set_tail(
    void    *ptr,
    int     alloc_size
)
{
    char *tail;
    tail = ((char*)ptr) + alloc_size - MARK_SIZE;
    memset(tail, MARK, MARK_SIZE);
}



//指定ポインタ（*mark）から指定サイズ分が、「0xCD」でなかった場合はエラー？（abort関数って何すんの？）
void
check_mark_sub(
    unsigned char   *mark,
    int             size
)
{
    int i;

    for (i = 0; i < size; i++)
    {
        if (mark[i] != MARK)
        {
            fprintf(stderr, "bad mark\n");
            abort();
        }
    }
}



//指定ヘッダのマーク領域と、ヘッダのサイズ情報から算出したフッタのマーク領域をチェック。
void
check_mark(
    Header  *header
)
{
    char    *tail;
    check_mark_sub(header->s.mark, (char*)&header[1] - (char*)header->s.mark);
    tail = ((char*)header) + header->s.size + sizeof(Header);
    check_mark_sub(tail, MARK_SIZE);
}



//領域確保を行い、メモリ管理コントローラに登録する。
void*
Memory_mallocFunction(
    Memory  controller,
    char    *filename,
    int     line,
    size_t  size
)
{
    void        *ptr;
    size_t      alloc_size;


    //確保領域のサイズを算出
    alloc_size = size + sizeof(Header) + MARK_SIZE;


    //領域を確保（失敗した場合はエラー処理）
    ptr = malloc(alloc_size);
    if (ptr == NULL)
        error_handler(controller, filename, line, "malloc");


    //確保領域の初期化、及びメモリ管理コントローラに紐付け
    memset(ptr, 0xCC, alloc_size);
    set_header(ptr, size, filename, line);
    set_tail(ptr, alloc_size);
    chain_block(controller, (Header*)ptr);
    ptr = (char*)ptr + sizeof(Header);

    //確保した領域の、実体部分（Header部分を取り除いた箇所）からのポインタを返却
    return ptr;
}



//指定ポインタを含むヘッダその他を再確保
//（そしてメモリ管理コントローラに再登録。）
void*
Memory_reallocFunction(
    Memory  controller,
    char    *filename,
    int     line,
    void    *ptr,
    size_t  size
)
{
    void    *new_ptr;
    size_t  alloc_size;
    void    *real_ptr;
    Header  old_header;
    int     old_size;


    //領域サイズを算出
    alloc_size = size + sizeof(Header) + MARK_SIZE;
    
    
    //指定されたポインタがNULLでない場合
    if (ptr != NULL)
    {
        real_ptr = (char*)ptr - sizeof(Header); //実際に確保されているポインタを算出して
        check_mark((Header*)real_ptr);          //そのポインタからのマーク領域をチェックし、
        old_header = *((Header*)real_ptr);      //検出した領域のヘッダ部分を取り出し、
        old_size = old_header.s.size;           //ヘッダのサイズを取得。
        unchain_block(controller, real_ptr);    //該当ポインタを管理から削除。
    }
    //指定されたポインタがNULLだった場合。
    else
    {
        real_ptr = NULL;    //実際に確保されているポインタはなし。
        old_size = 0;       //サイズは０に。
    }

    
    //実際に確保されているポインタに対し、算出した領域サイズでリアロケーション。
    //確保できなかった場合はエラー。
    new_ptr = realloc(real_ptr, alloc_size);
    if (new_ptr == NULL)
    {
        if (ptr == NULL)
            error_handler(controller, filename, line, "realloc(malloc)");
        else
        {
            error_handler(controller, filename, line, "realloc");
            free(real_ptr);
        }
    }


    //指定ポインタがNULLでなかった場合。
    if (ptr)
    {
        *((Header*)new_ptr) = old_header;               //新規確保した領域のヘッダに、古いヘッダの内容をコピー？
        ((Header*)new_ptr)->s.size = size;              //今回再度リアロケーションされるサイズを新規確保した領域のヘッダにセット。
        rechain_block(controller, (Header*)new_ptr);    //新規確保した領域を再度メモリ管理コントロールに登録。
        set_tail(new_ptr, alloc_size);                  //フッタを初期化
    }
    //指定ポインタがNULLだった場合。
    else
    {
        set_header(new_ptr, size, filename, line);      //新規確保したポインタのヘッダを初期化
        set_tail(new_ptr, alloc_size);                  //新規確保したポインタのヘッダを初期化
        chain_block(controller, (Header*)new_ptr);      //メモリ管理コントローラに登録
    }
    
    
    //返却するポインタを改めてnew_ptrとし、
    //返却先が使用するエリアの今回新規確保分のメモリを0xCCで初期化
    new_ptr = (char*)new_ptr + sizeof(Header);
    if (old_size < size)
        memset((char*)new_ptr + old_size, 0xCC, size - old_size);


    //新規確保したメモリへのポインタを返却
    return(new_ptr);
}



//指定された文字列を、メモリ管理コントローラで管理する。
char
*Memory_stringFunction(
    Memory  controller,
    char    *filename,
    int     line,
    char    *str
)
{
    char        *ptr;
    int         size;
    size_t      alloc_size;

    //「指定された文字列長＋１＋ヘッダサイズ＋マークサイズ」でメモリ確保。（確保できなかったらエラー）
    size = strlen(str) + 1;
    alloc_size = size + sizeof(Header) + MARK_SIZE;
    ptr = malloc(alloc_size);
    if (ptr == NULL)
        error_handler(controller, filename, line, "strdup");

    //確保した領域を0xCCで初期化し、メモリ管理コントローラに登録。
    //その後、確保したメモリ領域に指定された文字列のデータをコピーして返却。
    memset(ptr, 0xCC, alloc_size);
    set_header((Header*)ptr, size, filename, line);
    set_tail(ptr, alloc_size);
    chain_block(controller, (Header*)ptr);
    ptr = (char*)ptr + sizeof(Header);
    strcpy(ptr, str);

    return(ptr);
}



//指定されたポインタのメモリを解放する。
void
Memory_freeFunction(
    Memory  controller,
    void    *ptr
)
{
    void    *real_ptr;
    int     size;
    

    //指定ポインタがNULLの場合は何もせずに終了    
    if (ptr == NULL)
        return;


    //解放の前処理
    real_ptr = (char*)ptr - sizeof(Header);         //ヘッダ先頭へ移動
    check_mark((Header*)real_ptr);                  //マークの状況をチェック
    size = ((Header*)real_ptr)->s.size;             //ヘッダからサイズを取得して
    unchain_block(controller, real_ptr);            //メモリ管理コントローラから登録削除
    memset(real_ptr, 0xCC, size + sizeof(Header));  //解放対象領域を0xCCで初期化
    
    free(real_ptr);                                 //解放
}



//エラーハンドラの登録？
void
Memory_setErrorHandler(
    Memory              controller,
    MemoryErrorHandler  handler
)
{
    controller->error_handler = handler;
}



//失敗モード情報をセットする。
void
Memory_setFailMode(
    Memory          controller,
    MemoryFailMode  mode
)
{
    controller->fail_mode = mode;
}



//確保領域を指定されたファイルに出力
void
Memory_dumpBlocksFunction(Memory controller, FILE *fp)
{
    Header  *pos;
    int     counter = 0;
    int     i;

    for (pos = controller->block_header; pos; pos = pos->s.next) {
        check_mark(pos);
        fprintf(fp, "<DumpBlock> Counter=%04d / Address=%p / FileName=%s / Line=%d / Size=%d / Data=[",
            counter, 
            (char*)pos + sizeof(Header),
            pos->s.filename,
            pos->s.line,
            pos->s.size
        );
        for (i = 0; i < pos->s.size; i++)
        {
            if (isprint(*((char*)pos + sizeof(Header)+i)))
                fprintf(fp, "%c", *((char*)pos + sizeof(Header)+i));
            else
                fprintf(fp, ".");
        }
        fprintf(fp, "]\n");
        counter++;
    }
}



//指定ポインタのマークチェックを行う。
void
Memory_checkBlockFunction(
    Memory  controller,
    char    *filename,
    int     line,
    void    *p
)
{
    void *real_ptr = ((char*)p) - sizeof(Header);

    check_mark(real_ptr);
}



//全メモリのマークチェック？を行う。
void
Memory_checkAllBlocksFunction(
    Memory  controller,
    char    *filename, 
    int     line
)
{
    Header *pos;

    for (pos = controller->block_header; pos; pos = pos->s.next)
        check_mark(pos);
}



MemoryStorage
Memory_openStorageFunction(
    Memory  controller,
    char    *filename, 
    int     line,
    int     page_size
)
{
    MemoryStorage storage;

    storage = Memory_mallocFunction(controller, filename, line, sizeof(struct MemoryStorageTag));
    storage->page_list = NULL;
    assert(page_size >= 0);
    if (page_size > 0) {
        storage->current_page_size = page_size;
    } else {
        storage->current_page_size = DEFAULT_PAGE_SIZE;
    }

    return storage;
}



void*
Memory_mallocStorageFunction(
    Memory          controller,
    char            *filename, 
    int             line, 
    MemoryStorage   storage,
    size_t          size
)
{
    int                 cell_num;
    MemoryPage          *new_page;
    void                *p;

    cell_num = ((size - 1) / CELL_SIZE) + 1;

    if (storage->page_list != NULL
        && (storage->page_list->use_cell_num + cell_num
            < storage->page_list->cell_num)) {
        p = &(storage->page_list->cell[storage->page_list->use_cell_num]);
        storage->page_list->use_cell_num += cell_num;
    } else {
        int     alloc_cell_num;

        alloc_cell_num = larger(cell_num, storage->current_page_size);

        new_page = Memory_mallocFunction(controller, filename, line,
                                   sizeof(MemoryPage)
                                   + CELL_SIZE * (alloc_cell_num - 1));
        new_page->next = storage->page_list;
        new_page->cell_num = alloc_cell_num;
        storage->page_list = new_page;

        p = &(new_page->cell[0]);
        new_page->use_cell_num = cell_num;
    }

    return p;
}



void
Memory_disposeStorageFunction(
    Memory          controller, 
    MemoryStorage   storage
)
{
    MemoryPage  *temp;

    while (storage->page_list) {
        temp = storage->page_list->next;
        Memory_freeFunction(controller, storage->page_list);
        storage->page_list = temp;
    }
    Memory_freeFunction(controller, storage);
}
