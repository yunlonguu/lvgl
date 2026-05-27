#if LV_BUILD_TEST
#include "../lvgl.h"
#include "../../lvgl_private.h"

#include "unity/unity.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_group_count(void)
{
    lv_group_t * group_1 = lv_group_create();
    lv_group_t * group_2 = lv_group_create();

    TEST_ASSERT_EQUAL_UINT32(lv_group_get_count(), 2U);

    lv_group_delete(group_2);
    TEST_ASSERT_EQUAL_UINT32(lv_group_get_count(), 1U);

    lv_group_delete(group_1);
    TEST_ASSERT_EQUAL_UINT32(lv_group_get_count(), 0U);
}

void test_group_by_index(void)
{
    lv_group_t * group_1 = lv_group_create();
    lv_group_t * group_2 = lv_group_create();

    TEST_ASSERT_EQUAL_PTR(lv_group_by_index(2), NULL);
    TEST_ASSERT_EQUAL_PTR(lv_group_by_index(0), group_1);
    TEST_ASSERT_EQUAL_PTR(lv_group_by_index(1), group_2);

    lv_group_delete(group_1);
    lv_group_delete(group_2);
}

void test_group_obj_by_index(void)
{
    lv_group_t * group = lv_group_create();

    TEST_ASSERT_EQUAL_PTR(lv_group_get_obj_by_index(group, 0), NULL);

    lv_obj_t * obj_0 = lv_obj_create(NULL);
    lv_obj_t * obj_1 = lv_obj_create(NULL);
    lv_group_add_obj(group, obj_0);
    lv_group_add_obj(group, obj_1);

    TEST_ASSERT_EQUAL_PTR(lv_group_get_obj_by_index(group, 0), obj_0);
    TEST_ASSERT_EQUAL_PTR(lv_group_get_obj_by_index(group, 1), obj_1);

    lv_group_remove_obj(obj_0);
    TEST_ASSERT_EQUAL_PTR(lv_group_get_obj_by_index(group, 0), obj_1);
    TEST_ASSERT_EQUAL_PTR(lv_group_get_obj_by_index(group, 1), NULL);
}

static void count_defocused_cb(lv_event_t * e)
{
    uint32_t * counter = lv_event_get_user_data(e);
    (*counter)++;
}

void test_group_remove_focused_obj_sends_defocused_when_others_unfocusable(void)
{
    lv_obj_t * screen = lv_screen_active();
    lv_group_t * group = lv_group_create();

    lv_obj_t * obj_hidden  = lv_obj_create(screen);
    lv_obj_t * obj_disabled = lv_obj_create(screen);
    lv_obj_t * obj_focused = lv_obj_create(screen);

    lv_group_add_obj(group, obj_hidden);
    lv_group_add_obj(group, obj_disabled);
    lv_group_add_obj(group, obj_focused);

    lv_obj_add_flag(obj_hidden, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_state(obj_disabled, LV_STATE_DISABLED);

    lv_group_focus_obj(obj_focused);
    TEST_ASSERT_EQUAL_PTR(lv_group_get_focused(group), obj_focused);

    uint32_t defocused_count = 0;
    lv_obj_add_event_cb(obj_focused, count_defocused_cb, LV_EVENT_DEFOCUSED, &defocused_count);

    lv_group_remove_obj(obj_focused);

    uint32_t final_count = defocused_count;

    lv_obj_delete(obj_hidden);
    lv_obj_delete(obj_disabled);
    lv_obj_delete(obj_focused);
    lv_group_delete(group);

    TEST_ASSERT_EQUAL_UINT32(1U, final_count);
}

#endif
