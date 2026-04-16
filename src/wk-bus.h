#ifndef WK_BUS_H
#define WK_BUS_H
#include <string.h>
#include <wk-err.h>
#include <wk-box.h>
#include <wk-array.h>

/* 声明总线 */
#define wk_bus_declare(CamelName, udline_name) \
extern WKArray *_##udline_name##_bus_; \
\
typedef struct { \
        const char *type; \
        CamelName action; \
} CamelName##Slot; \
\
void udline_name##_bus_connect(const char *type, CamelName action); \
/* 这行尾巴是为了让宏调用语句能像函数那样以分号结尾 */ \
/* typedef int _this_type_never_used_##__LINE__##_ */ \
/* 或者用 C11 支持的静态断言 */ \
_Static_assert(1 == 1, "为了宏调用能加分号，我也是拼了")

/* 定义总线 */
#define wk_bus_define(CamelName, udline_name) \
WKArray *_##udline_name##_bus_ = NULL; \
void udline_name##_bus_connect(const char *type, CamelName action) { \
        if (!_##udline_name##_bus_) _##udline_name##_bus_ = wk_array(CamelName##Slot *); \
        /* 检查是否槽位是否已被占用 */ \
        CamelName##Slot *old_slot = NULL; \
        for (size_t i = 0; i < _##udline_name##_bus_->n; i++) { \
                CamelName##Slot *slot_i = wk_array_get(_##udline_name##_bus_, i, CamelName##Slot *); \
                if (strcmp(slot_i->type, type) == 0) { \
                        old_slot = slot_i; \
                        break; \
                } \
        } \
        if (old_slot) { \
                old_slot->action = action; \
        } else { \
                CamelName##Slot *slot = malloc(sizeof(CamelName##Slot)); \
                if (!slot) wk_err("failed to create CamelName##Slot object"); \
                *slot = (CamelName##Slot){type, action}; \
                wk_array_add(_##udline_name##_bus_, slot, CamelName##Slot *); \
        } \
        wk_fallback; \
} \
/* 这行尾巴是为了让宏调用语句能像函数那样以分号结尾 */ \
/* typedef int _this_type_never_used_##__LINE__##_ */ \
/* 或者用 C11 支持的静态断言 */ \
_Static_assert(1 == 1, "为了宏调用能加分号，我也是拼了")

/*----------------*/
/* 下面是常用的总线 */
/*----------------*/
/* 释放对象 */
typedef void (*WKFree)(WKBox *a);
void wk_free(WKBox *a);
wk_bus_declare(WKFree, wk_free);

/* 计算散列值 */
typedef size_t (*WKHash)(WKBox *a);
size_t wk_hash(WKBox *a);
wk_bus_declare(WKHash, wk_hash);

/* 测试两个对象是否相等 */
typedef bool (*WKEqual)(WKBox *a, WKBox *b);
bool wk_equal(WKBox *a, WKBox *b);
wk_bus_declare(WKEqual, wk_equal);

/* 比较两个对象大小 */
/* a < b 返回 -1, a == b 返回 0, a > b 返回 1；若 a 和 b 无法比较，则返回 -2 */
typedef int (*WKCompare)(WKBox *a, WKBox *b);
int wk_compare(WKBox *a, WKBox *b);
wk_bus_declare(WKCompare, wk_compare);

/* 浮点数比较相等时，需要用误差容忍度 */
#include <float.h>
#define wk_flt_eps (10 * FLT_EPSILON)
#define wk_dbl_eps (10 * DBL_EPSILON)

#endif

