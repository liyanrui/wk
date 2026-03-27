#ifndef WK_BUS_H
#define WK_BUS_H
#include <string.h>
#include <wk-err.h>
#include <wk-box.h>
#include <wk-array.h>

/* 用于包含函数内部只执行一次的代码片段 */
#define wk_first_run_begin \
        static bool _wk_first_run_ = true; \
        if (_wk_first_run_) { \
        _Static_assert(1 == 1, "为了宏调用能加分号，我也是拼了")

#define wk_first_run_end \
        _wk_first_run_ = false; \
} \
_Static_assert(1 == 1, "为了宏调用能加分号，我也是拼了")

/* 声明总线 */
#define wk_bus_declare(CamelName, udline_name) \
extern WKArray *_##udline_name##_bus_; \
\
typedef struct { \
        const char *type; \
        CamelName action; \
} CamelName##Slot; \
/* 这行尾巴是为了让宏调用语句能像函数那样以分号结尾 */ \
/* typedef int _this_type_never_used_##__LINE__##_ */ \
/* 或者用 C11 支持的静态断言 */ \
_Static_assert(1 == 1, "为了宏调用能加分号，我也是拼了")

/* 定义总线 */
#define wk_bus_define(CamelName, udline_name) \
WKArray *_##udline_name##_bus_ = NULL; \
void udline_name##_bus_connect(const char *type, CamelName action) { \
        if (!_##udline_name##_bus_) _##udline_name##_bus_ = wk_array(CamelName##Slot *); \
        CamelName##Slot *slot = malloc(sizeof(CamelName##Slot)); \
        if (!slot) wk_err("failed to create CamelName##Slot object"); \
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
                *slot = (CamelName##Slot){type, action}; \
                wk_array_add(_##udline_name##_bus_, slot, CamelName##Slot *); \
        } \
        wk_fallback; \
} \
/* 这行尾巴是为了让宏调用语句能像函数那样以分号结尾 */ \
/* typedef int _this_type_never_used_##__LINE__##_ */ \
/* 或者用 C11 支持的静态断言 */ \
_Static_assert(1 == 1, "为了宏调用能加分号，我也是拼了")

#endif
