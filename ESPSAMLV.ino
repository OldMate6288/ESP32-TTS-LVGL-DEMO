// ESP8266SAM TextToSpeech LVGL demo by OldMate6288
// https://github.com/OldMate6288

#include <lvgl.h>
#include <ESP8266SAM.h>
#include <AudioOutputI2S.h>
#include "ui.h"
#include "lgfx_ESP32_2432S028.h"

AudioOutputI2S *out = NULL;

/*Change to your screen resolution*/
static const uint32_t screenWidth  = 240;
static const uint32_t screenHeight = 320;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
   uint32_t w = ( area->x2 - area->x1 + 1 );
   uint32_t h = ( area->y2 - area->y1 + 1 );

   tft.startWrite();
   tft.setAddrWindow( area->x1, area->y1, w, h );
   //tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
   tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
   tft.endWrite();

   lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
   uint16_t touchX, touchY;
   bool touched = tft.getTouch( &touchX, &touchY);
   if( !touched )
   {
      data->state = LV_INDEV_STATE_REL;
   }
   else
   {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touchX;
      data->point.y = touchY;

      Serial.print( "Data x " );
      Serial.println( touchX );

      Serial.print( "Data y " );
      Serial.println( touchY );
   }
}

///////////////////// SCREENS ////////////////////

void ui_Screen1_screen_init(void)
{
ui_Screen1 = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Screen1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_TextArea1 = lv_textarea_create(ui_Screen1);
lv_obj_set_width( ui_TextArea1, 200);
lv_obj_set_height( ui_TextArea1, 70);
lv_obj_set_x( ui_TextArea1, 0 );
lv_obj_set_y( ui_TextArea1, -80 );
lv_obj_set_align( ui_TextArea1, LV_ALIGN_CENTER );
lv_textarea_set_placeholder_text(ui_TextArea1,"Type Text Here...");

ui_Keyboard1 = lv_keyboard_create(ui_Screen1);
lv_obj_set_width( ui_Keyboard1, 236);
lv_obj_set_height( ui_Keyboard1, 120);
lv_obj_set_x( ui_Keyboard1, 0 );
lv_obj_set_y( ui_Keyboard1, 100 );
lv_obj_set_align( ui_Keyboard1, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Keyboard1, LV_OBJ_FLAG_HIDDEN );   /// Flags

ui_Button1 = lv_btn_create(ui_Screen1);
lv_obj_set_width( ui_Button1, 65);
lv_obj_set_height( ui_Button1, 25);
lv_obj_set_x( ui_Button1, 0 );
lv_obj_set_y( ui_Button1, -20 );
lv_obj_set_align( ui_Button1, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Button1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_Button1, lv_color_hex(0x0054FF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Button1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label1 = lv_label_create(ui_Button1);
lv_obj_set_width( ui_Label1, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label1, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label1,"Speak");

ui_Label2 = lv_label_create(ui_Screen1);
lv_obj_set_width( ui_Label2, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label2, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label2, 0 );
lv_obj_set_y( ui_Label2, -140 );
lv_obj_set_align( ui_Label2, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label2,"ESP32 TextToSpeech");
lv_obj_set_style_text_align(ui_Label2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Label2, &lv_font_montserrat_22, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label3 = lv_label_create(ui_Screen1);
lv_obj_set_width( ui_Label3, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label3, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label3, 0 );
lv_obj_set_y( ui_Label3, 140 );
lv_obj_set_align( ui_Label3, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label3,"https://github.com/OldMate6288");
lv_obj_set_style_text_align(ui_Label3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Label3, &lv_font_montserrat_12, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_add_event_cb(ui_TextArea1, ui_event_TextArea1, LV_EVENT_ALL, NULL);
lv_keyboard_set_textarea(ui_Keyboard1,ui_TextArea1);
lv_obj_add_event_cb(ui_Button1, ui_event_Button1, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_Screen1, ui_event_Screen1, LV_EVENT_ALL, NULL);

}

///////////////////// FUNCTIONS ////////////////////

void ui_event_Screen1( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_flag_modify( ui_Keyboard1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
}
if ( event_code == LV_EVENT_CLICKED) {
      _ui_flag_modify( ui_Label3, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
}
}
void ui_event_TextArea1( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_flag_modify( ui_Keyboard1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
}
if ( event_code == LV_EVENT_CLICKED) {
      _ui_flag_modify( ui_Label3, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
}
}
void ui_event_Button1(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (event_code == LV_EVENT_CLICKED) {
        const char *text = lv_textarea_get_text(ui_TextArea1);
        ESP8266SAM *sam = new ESP8266SAM;
        sam->Say(out, text);
        delay(500);
        delete sam;
    }
}

void setup()
{
   Serial.begin(115200);

   out = new AudioOutputI2S(0, 2, 8, -1);
   out->begin();

   tft.begin();        
   tft.setRotation(2);
   tft.setBrightness(255);
   uint16_t calData[] = { 239, 3926, 233, 265, 3856, 3896, 3714, 308};
   tft.setTouchCalibrate(calData);
   lv_init();
   lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

   /*Initialize the display*/
   static lv_disp_drv_t disp_drv;
   lv_disp_drv_init(&disp_drv);

   /*Change the following line to your display resolution*/
   disp_drv.hor_res = screenWidth;
   disp_drv.ver_res = screenHeight;
   disp_drv.flush_cb = my_disp_flush;
   disp_drv.draw_buf = &draw_buf;
   lv_disp_drv_register(&disp_drv);

   /*Initialize the (dummy) input device driver*/
   static lv_indev_drv_t indev_drv;
   lv_indev_drv_init(&indev_drv);
   indev_drv.type = LV_INDEV_TYPE_POINTER;
   indev_drv.read_cb = my_touchpad_read;
   lv_indev_drv_register(&indev_drv);

   ui_init();
}

void loop()
{
   lv_timer_handler(); /* let the GUI do its work */
   delay( 5 );
}