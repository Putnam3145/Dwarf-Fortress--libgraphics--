#include <assert.h>
#include <iostream>
#include "../render/interface.hpp"
#include "../render/init.hpp"

using namespace std;
using namespace widgets;

bool scrollbarst::handle_events(std::set<InterfaceKey>& events, int32_t& scroll_position, bool& scrolling)
{
  if (events.count(INTERFACEKEY_CONTEXT_SCROLL_UP))
  {
    events.clear();
    scrolling = false;

    --sel;
    if (sel < sel_min)sel = sel_min;
    scroll_position = sel;
    return true;
  }
  if (events.count(INTERFACEKEY_CONTEXT_SCROLL_DOWN))
  {
    events.clear();
    scrolling = false;

    ++sel;
    if (sel > sel_max - page_size + 1)sel = sel_max - page_size + 1;
    scroll_position = sel;
    return true;
  }
  if (events.count(INTERFACEKEY_CONTEXT_SCROLL_PAGEUP))
  {
    events.clear();
    scrolling = false;

    sel -= page_size;
    if (sel < sel_min)sel = sel_min;
    scroll_position = sel;
    return true;
  }
  if (events.count(INTERFACEKEY_CONTEXT_SCROLL_PAGEDOWN))
  {
    events.clear();
    scrolling = false;

    sel += page_size;
    if (sel > sel_max - page_size + 1)sel = sel_max - page_size + 1;
    scroll_position = sel;
    return true;
  }

  return false;
}

void scrollbarst::handle_initial_click(int32_t mouse_y, int32_t& scroll_position, bool& scrolling)
{
  scrolling = false;

    //top arrow
  if (mouse_y == print_sy - 1)
  {
    --sel;
    if (sel < sel_min)sel = sel_min;
    scroll_position = sel;
    return;
  }
    //bottom arrow
  if (mouse_y == print_ey + 1)
  {
    ++sel;
    if (sel > sel_max - page_size + 1)sel = sel_max - page_size + 1;
    scroll_position = sel;
    return;
  }
    //hit the scroller
  if (mouse_y >= scroller_sy && mouse_y <= scroller_ey)
  {
    scrolling = true;
    return;
  }
    //page up
  if (mouse_y < scroller_sy)
  {
    sel -= page_size;
    if (sel < sel_min)sel = sel_min;
    scroll_position = sel;
    return;
  }
    //page down
  if (mouse_y > scroller_ey)
  {
    sel += page_size;
    if (sel > sel_max - page_size + 1)sel = sel_max - page_size + 1;
    scroll_position = sel;
    return;
  }
}

void scrollbarst::render(int32_t mouse_x, int32_t mouse_y, int32_t bx, bool active, uint32_t display_flag)
{
  if (display_flag & SCROLLBAR_DISPLAY_FLAG_BASIC_SET)
  {
    int32_t px, py;
    for (px = bx; px <= bx + 1; ++px)
    {
            for (py = print_sy - 1; py <= print_ey + 1; ++py)//+/-1 for arrows at top bottom
            {
              gps.locate(py, px);
                //************************** PRESSED SCROLL BUTTONS
                    //we have texpos_scrollbar_up/down_pressed but they aren't used yet
                        //hard to maintain variable since the scrollbarst doesn't persist anywhere
              if (py == print_sy - 1)
              {
                if (mouse_y == py && mouse_x >= bx && mouse_x <= bx + 1)gps.add_lower_tile(::init.texpos_scrollbar_up_hover[px - bx]);
                else gps.add_lower_tile(::init.texpos_scrollbar[px - bx][0]);
              }
              else if (py == print_ey + 1)
              {
                if (mouse_y == py && mouse_x >= bx && mouse_x <= bx + 1)gps.add_lower_tile(::init.texpos_scrollbar_down_hover[px - bx]);
                else gps.add_lower_tile(::init.texpos_scrollbar[px - bx][2]);
              }
              else gps.add_lower_tile(::init.texpos_scrollbar[px - bx][1]);

              if (::init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))gps.addchar(0);
            }
          }

          bool hover = ((mouse_x >= bx && mouse_x <= bx + 1 && mouse_y >= print_sy && mouse_y <= print_ey) || active);

          if (scroller_ey - 1 <= scroller_sy)
          {
            if (hover)
            {
              gps.locate(scroller_sy, bx);
              gps.add_tile(::init.texpos_scrollbar_small_scroller_hover[0][0], 0);
              gps.locate(scroller_sy, bx + 1);
              gps.add_tile(::init.texpos_scrollbar_small_scroller_hover[1][0], 0);
              gps.locate(scroller_ey, bx);
              gps.add_tile(::init.texpos_scrollbar_small_scroller_hover[0][1], 0);
              gps.locate(scroller_ey, bx + 1);
              gps.add_tile(::init.texpos_scrollbar_small_scroller_hover[1][1], 0);
            }
            else
            {
              gps.locate(scroller_sy, bx);
              gps.add_tile(::init.texpos_scrollbar_small_scroller[0][0], 0);
              gps.locate(scroller_sy, bx + 1);
              gps.add_tile(::init.texpos_scrollbar_small_scroller[1][0], 0);
              gps.locate(scroller_ey, bx);
              gps.add_tile(::init.texpos_scrollbar_small_scroller[0][1], 0);
              gps.locate(scroller_ey, bx + 1);
              gps.add_tile(::init.texpos_scrollbar_small_scroller[1][1], 0);
            }
          }
          else if ((scroller_ey - scroller_sy) % 2 == 0)
          {
            int32_t mid = (scroller_sy + scroller_ey) / 2;
            int32_t py;
            for (py = scroller_sy; py <= scroller_ey; ++py)
            {
              if (py == mid)
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(::init.texpos_scrollbar_center_scroller_hover[0], 0);
                else gps.add_tile(::init.texpos_scrollbar_center_scroller[0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(::init.texpos_scrollbar_center_scroller_hover[1], 0);
                else gps.add_tile(::init.texpos_scrollbar_center_scroller[1], 0);
              }
              else if (py == scroller_sy)
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(::init.texpos_scrollbar_top_scroller_hover[0], 0);
                else gps.add_tile(::init.texpos_scrollbar_top_scroller[0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(::init.texpos_scrollbar_top_scroller_hover[1], 0);
                else gps.add_tile(::init.texpos_scrollbar_top_scroller[1], 0);
              }
              else if (py == scroller_ey)
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(::init.texpos_scrollbar_bottom_scroller_hover[0], 0);
                else gps.add_tile(::init.texpos_scrollbar_bottom_scroller[0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(::init.texpos_scrollbar_bottom_scroller_hover[1], 0);
                else gps.add_tile(::init.texpos_scrollbar_bottom_scroller[1], 0);
              }
              else
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(::init.texpos_scrollbar_blank_scroller_hover[0], 0);
                else gps.add_tile(::init.texpos_scrollbar_blank_scroller[0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(::init.texpos_scrollbar_blank_scroller_hover[1], 0);
                else gps.add_tile(::init.texpos_scrollbar_blank_scroller[1], 0);
              }
            }
          }
          else
          {
            int32_t mid = (scroller_sy + scroller_ey) / 2;
            int32_t py;
            for (py = scroller_sy; py <= scroller_ey; ++py)
            {
              if (py == mid)
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(::init.texpos_scrollbar_offcenter_scroller_hover[0][0], 0);
                else gps.add_tile(::init.texpos_scrollbar_offcenter_scroller[0][0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(::init.texpos_scrollbar_offcenter_scroller_hover[1][0], 0);
                else gps.add_tile(::init.texpos_scrollbar_offcenter_scroller[1][0], 0);
              }
              else if (py == mid + 1)
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(::init.texpos_scrollbar_offcenter_scroller_hover[0][1], 0);
                else gps.add_tile(::init.texpos_scrollbar_offcenter_scroller[0][1], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(::init.texpos_scrollbar_offcenter_scroller_hover[1][1], 0);
                else gps.add_tile(::init.texpos_scrollbar_offcenter_scroller[1][1], 0);
              }
              else if (py == scroller_sy)
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(::init.texpos_scrollbar_top_scroller_hover[0], 0);
                else gps.add_tile(::init.texpos_scrollbar_top_scroller[0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(::init.texpos_scrollbar_top_scroller_hover[1], 0);
                else gps.add_tile(::init.texpos_scrollbar_top_scroller[1], 0);
              }
              else if (py == scroller_ey)
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(::init.texpos_scrollbar_bottom_scroller_hover[0], 0);
                else gps.add_tile(::init.texpos_scrollbar_bottom_scroller[0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(::init.texpos_scrollbar_bottom_scroller_hover[1], 0);
                else gps.add_tile(::init.texpos_scrollbar_bottom_scroller[1], 0);
              }
              else
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(::init.texpos_scrollbar_blank_scroller_hover[0], 0);
                else gps.add_tile(::init.texpos_scrollbar_blank_scroller[0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(::init.texpos_scrollbar_blank_scroller_hover[1], 0);
                else gps.add_tile(::init.texpos_scrollbar_blank_scroller[1], 0);
              }
            }
          }
        }
        else
        {
          int32_t px, py;
          for (px = bx; px <= bx + 1; ++px)
          {
            for (py = print_sy - 1; py <= print_ey + 1; ++py)//+/-1 for arrows at top bottom
            {
              gps.locate(py, px);
                //************************** PRESSED SCROLL BUTTONS
                    //we have texpos_scrollbar_up/down_pressed but they aren't used yet
              if (py == print_sy - 1)
              {
                if (mouse_y == py && mouse_x >= bx && mouse_x <= bx + 1)gps.add_lower_tile(gps.texpos_scrollbar_up_hover[px - bx]);
                else gps.add_lower_tile(gps.texpos_scrollbar[px - bx][0]);
              }
              else if (py == print_ey + 1)
              {
                if (mouse_y == py && mouse_x >= bx && mouse_x <= bx + 1)gps.add_lower_tile(gps.texpos_scrollbar_down_hover[px - bx]);
                else gps.add_lower_tile(gps.texpos_scrollbar[px - bx][2]);
              }
              else gps.add_lower_tile(gps.texpos_scrollbar[px - bx][1]);

              if (::init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))gps.addchar(0);
            }
          }

          bool hover = ((mouse_x >= bx && mouse_x <= bx + 1 && mouse_y >= print_sy && mouse_y <= print_ey) || active);

          if (scroller_ey - 1 <= scroller_sy)
          {
            if (hover)
            {
              gps.locate(scroller_sy, bx);
              gps.add_tile(gps.texpos_scrollbar_small_scroller_hover[0][0], 0);
              gps.locate(scroller_sy, bx + 1);
              gps.add_tile(gps.texpos_scrollbar_small_scroller_hover[1][0], 0);
              gps.locate(scroller_ey, bx);
              gps.add_tile(gps.texpos_scrollbar_small_scroller_hover[0][1], 0);
              gps.locate(scroller_ey, bx + 1);
              gps.add_tile(gps.texpos_scrollbar_small_scroller_hover[1][1], 0);
            }
            else
            {
              gps.locate(scroller_sy, bx);
              gps.add_tile(gps.texpos_scrollbar_small_scroller[0][0], 0);
              gps.locate(scroller_sy, bx + 1);
              gps.add_tile(gps.texpos_scrollbar_small_scroller[1][0], 0);
              gps.locate(scroller_ey, bx);
              gps.add_tile(gps.texpos_scrollbar_small_scroller[0][1], 0);
              gps.locate(scroller_ey, bx + 1);
              gps.add_tile(gps.texpos_scrollbar_small_scroller[1][1], 0);
            }
          }
          else if ((scroller_ey - scroller_sy) % 2 == 0)
          {
            int32_t mid = (scroller_sy + scroller_ey) / 2;
            int32_t py;
            for (py = scroller_sy; py <= scroller_ey; ++py)
            {
              if (py == mid)
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(gps.texpos_scrollbar_center_scroller_hover[0], 0);
                else gps.add_tile(gps.texpos_scrollbar_center_scroller[0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(gps.texpos_scrollbar_center_scroller_hover[1], 0);
                else gps.add_tile(gps.texpos_scrollbar_center_scroller[1], 0);
              }
              else if (py == scroller_sy)
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(gps.texpos_scrollbar_top_scroller_hover[0], 0);
                else gps.add_tile(gps.texpos_scrollbar_top_scroller[0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(gps.texpos_scrollbar_top_scroller_hover[1], 0);
                else gps.add_tile(gps.texpos_scrollbar_top_scroller[1], 0);
              }
              else if (py == scroller_ey)
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(gps.texpos_scrollbar_bottom_scroller_hover[0], 0);
                else gps.add_tile(gps.texpos_scrollbar_bottom_scroller[0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(gps.texpos_scrollbar_bottom_scroller_hover[1], 0);
                else gps.add_tile(gps.texpos_scrollbar_bottom_scroller[1], 0);
              }
              else
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(gps.texpos_scrollbar_blank_scroller_hover[0], 0);
                else gps.add_tile(gps.texpos_scrollbar_blank_scroller[0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(gps.texpos_scrollbar_blank_scroller_hover[1], 0);
                else gps.add_tile(gps.texpos_scrollbar_blank_scroller[1], 0);
              }
            }
          }
          else
          {
            int32_t mid = (scroller_sy + scroller_ey) / 2;
            int32_t py;
            for (py = scroller_sy; py <= scroller_ey; ++py)
            {
              if (py == mid)
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(gps.texpos_scrollbar_offcenter_scroller_hover[0][0], 0);
                else gps.add_tile(gps.texpos_scrollbar_offcenter_scroller[0][0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(gps.texpos_scrollbar_offcenter_scroller_hover[1][0], 0);
                else gps.add_tile(gps.texpos_scrollbar_offcenter_scroller[1][0], 0);
              }
              else if (py == mid + 1)
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(gps.texpos_scrollbar_offcenter_scroller_hover[0][1], 0);
                else gps.add_tile(gps.texpos_scrollbar_offcenter_scroller[0][1], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(gps.texpos_scrollbar_offcenter_scroller_hover[1][1], 0);
                else gps.add_tile(gps.texpos_scrollbar_offcenter_scroller[1][1], 0);
              }
              else if (py == scroller_sy)
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(gps.texpos_scrollbar_top_scroller_hover[0], 0);
                else gps.add_tile(gps.texpos_scrollbar_top_scroller[0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(gps.texpos_scrollbar_top_scroller_hover[1], 0);
                else gps.add_tile(gps.texpos_scrollbar_top_scroller[1], 0);
              }
              else if (py == scroller_ey)
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(gps.texpos_scrollbar_bottom_scroller_hover[0], 0);
                else gps.add_tile(gps.texpos_scrollbar_bottom_scroller[0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(gps.texpos_scrollbar_bottom_scroller_hover[1], 0);
                else gps.add_tile(gps.texpos_scrollbar_bottom_scroller[1], 0);
              }
              else
              {
                gps.locate(py, bx);
                if (hover)gps.add_tile(gps.texpos_scrollbar_blank_scroller_hover[0], 0);
                else gps.add_tile(gps.texpos_scrollbar_blank_scroller[0], 0);
                gps.locate(py, bx + 1);
                if (hover)gps.add_tile(gps.texpos_scrollbar_blank_scroller_hover[1], 0);
                else gps.add_tile(gps.texpos_scrollbar_blank_scroller[1], 0);
              }
            }
          }
        }
      }

      widget::widget() {
        visible = true;
        rel_x = 0;
        rel_y = 0;
        name = "";
      }

      void draw_nineslice(int32_t* texpos, int sy, int sx, int ey, int ex, bool override) {
    // corners
        gps.locate(sy, sx);
        gps.add_lower_tile(texpos[0]);
        gps.locate(sy, ex);
        gps.add_lower_tile(texpos[2]);
        gps.locate(ey, sx);
        gps.add_lower_tile(texpos[6]);
        gps.locate(ey, ex);
        gps.add_lower_tile(texpos[8]);
    // top and bottom
        for (auto bx = sx + 1; bx <= ex - 1; bx++) {
          gps.locate(sy, bx);
          gps.add_lower_tile(texpos[1]);
          gps.locate(ey, bx);
          gps.add_lower_tile(texpos[7]);
        }
    // sides and middle
        for (auto by = sy + 1; by <= ey - 1; by++) {
          gps.locate(by, sx);
          gps.add_lower_tile(texpos[3]);
          gps.locate(by, ex);
          gps.add_lower_tile(texpos[5]);
          for (auto bx = sx + 1; bx <= ex - 1; bx++) {
            gps.locate(by, bx);
            gps.add_lower_tile(texpos[4]);
          }
        }
        if (override && ::init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS)) {
          for (auto by = sy; by <= ey; by++) {
            for (auto bx = sx; bx <= ex; bx++) {
              gps.locate(by, bx);
              gps.addchar(0);
            }
          }
        }
      }

      void widget::set_extents(int sy, int ey, int sx, int ex) {
        rect.x = sx;
        rect.y = sy;
        rect.w = ex - sx;
        rect.h = ey - sy;
      }

      void widget::resize(int h, int w) {
        rect.w = w;
        rect.h = h;
      }

      void widget::move(int y, int x) {
        rect.x = x;
        rect.y = y;
      }

      void widget::move_relative(widget& other) {
        rect.x = other.rect.x + rel_x;
        rect.y = other.rect.y + rel_y;
      }

      void widget::move_relative(widget& other, int y, int x) {
        rel_x = x;
        rel_y = y;
        rect.x = other.rect.x + x;
        rect.y = other.rect.y + y;
      }

      void widget::coords(int32_t& sy, int32_t& sx, int32_t& ey, int32_t& ex) {
        sx = rect.x;
        sy = rect.y;
        ex = sx + rect.w - 1;
        ey = sy + rect.h - 1;
      }

      bool widget::inside(int32_t y, int32_t x) {
        return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
      }

      void container::feed(std::set<InterfaceKey>& events) {
        for (auto& child : children) {
          if (child->visible)
            child->feed(events);
        }
      }

      void container::logic() {
        for (auto& child : children) {
          if (child->visible)
            child->logic();
        }
      }

      void container::render() {
        for (auto& child : children) {
          if (child->visible)
            child->render();
        }
      }

      void container::arrange()
      {
        for (auto& child : children) {
          child->move_relative(*this);
        child->arrange(); // want to do these even if they're invisible
      }
    }

    void rows_container::arrange() {
      int32_t y = 0;
      for (auto& child : children) {
        if (child->visible) {
          child->move_relative(*this, y, 0);
          y += child->rect.h + padding;
        }
        child->arrange();
      }
    }

    void columns_container::arrange() {
      int32_t x = rect.x;
      for (auto& child : children) {
        if (child->visible) {
          child->move_relative(*this, 0, padding);
          x += child->rect.w + padding;
        }
        child->arrange();
      }
    }

    void scroll_rows::feed(set<InterfaceKey>& events) {
      int32_t tmx = -1, tmy = -1;
      if (enabler.tracking_on)
      {
        gps.get_mouse_text_coords(tmx, tmy);
      }
      scrollbar.init(scroll, 0, (int32_t)children.size() - 1, num_visible, rect.y, rect.y + rect.h);
      if (scrollbar.handle_events(events, scroll, scrolling))return;
      rows_container::feed(events);
    }

    void scroll_rows::render() {
      for (auto& child : children) {
        if (child->visible)
          child->render();
      }
      int32_t tmx = -1, tmy = -1;
      if (enabler.tracking_on)
      {
        gps.get_mouse_text_coords(tmx, tmy);
      }
      scrollbar.init(scroll, 0, (int32_t)children.size() - 1, num_visible, rect.y, rect.y+rect.h);
      scrollbar.render(tmx, tmy, rect.x+rect.w, scroll, SCROLLBAR_DISPLAY_FLAG_BASIC_SET);
    }

    void scroll_rows::arrange() {
      std::for_each(children.begin(), children.end(), [](auto& child) {child->visible = true; });
      int32_t sy = 0;
      num_visible = 0;
      for (auto i = 0; i < children.size(); i++) {
        auto &child = children[i];
        child->visible = false;
        if (i>=scroll && child->rect.h + sy <= rect.h) {
          child->visible = true;
          num_visible++;
          child->move_relative(*this, sy, 0);
          sy += child->rect.h + padding;
        }
      }
    }

    void tabs::add_tab_label(const char* s) {
      tab_labels.push_back(string(s));
    }

    void tabs::add_tab_label(string& s) {
      tab_labels.push_back(s);
    }

    bool tabs::switch_tab(size_t which) {
      if (which == cur_idx || which<0) return false;
      cur_idx = which;
      arrange();
      return true;
    }

    void tabs::feed(set<InterfaceKey>& events) {
      if (enabler.tracking_on && enabler.mouse_lbut) {
        int32_t tab_sx, tab_ex;
        tab_sx = rect.x;
        int32_t mx = 0, my = 0;
        gps.get_mouse_text_coords(mx, my);
        if (my >= rect.y && my <= rect.y + 1) {
          for (auto i = 0; i < tab_labels.size(); i++) {
            int tab_width = (int32_t)tab_labels[i].length() + 3;
            tab_ex = tab_sx + tab_width;
            if (mx >= tab_sx && mx <= tab_ex) {
              bool succeeded = switch_tab(i);
              if (succeeded) {
                enabler.mouse_lbut = false;
              }
              break;
            }
            tab_sx += tab_width + 1;
          }
        }
      }
      if(cur_idx<children.size())
        children[cur_idx]->feed(events);
    }

    void tabs::render() {
      int32_t tab_sx, tab_ex;
      tab_sx = rect.x;
      for (auto i = 0; i < tab_labels.size(); i++) {
        tab_ex = tab_sx + (int32_t)tab_labels[i].length() + 3;
        auto texpos = cur_idx == i ? init.texpos_tab_selected : init.texpos_tab_unselected;
        // left border
        gps.locate(rect.y, tab_sx);
        gps.add_lower_tile(texpos[0][0]);
        gps.locate(rect.y + 1, tab_sx);
        gps.add_lower_tile(texpos[0][1]);
        gps.locate(rect.y, tab_sx + 1);
        gps.add_lower_tile(texpos[1][0]);
        gps.locate(rect.y + 1, tab_sx + 1);
        // right border
        gps.add_lower_tile(texpos[1][1]);
        gps.locate(rect.y, tab_ex);
        gps.add_lower_tile(texpos[4][0]);
        gps.locate(rect.y + 1, tab_ex);
        gps.add_lower_tile(texpos[4][1]);
        gps.locate(rect.y, tab_ex - 1);
        gps.add_lower_tile(texpos[3][0]);
        gps.locate(rect.y + 1, tab_ex - 1);
        gps.add_lower_tile(texpos[3][1]);
        // and the rest
        for (auto px = tab_sx + 2; px <= tab_ex - 2; px++) {
          gps.locate(rect.y, px);
          gps.add_lower_tile(texpos[2][0]);
          gps.locate(rect.y + 1, px);
          gps.add_lower_tile(texpos[2][1]);
        }
        // labels
        if (!(::init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS)))
        {
          gps.locate(rect.y + 1, tab_sx + 2);
          gps.addst(tab_labels[i]);
        }
        else {
          gps.locate(rect.y, tab_sx + 2);
          gps.addst_flag(tab_labels[i], justify_left, 0, SCREENTEXPOS_FLAG_TOP_OF_TEXT);
          gps.locate(rect.y + 1, tab_sx + 2);
          gps.addst_flag(tab_labels[i], justify_left, 0, SCREENTEXPOS_FLAG_BOTTOM_OF_TEXT);
        }
        tab_sx += (int32_t)tab_labels[i].length() + 4;
      }
      if (cur_idx < children.size())
        children[cur_idx]->render();
    }

    void tabs::arrange() {
      for (auto& tab : children) {
        tab->visible = false;
      }
      if (cur_idx>=children.size()) return;
      auto cur_tab = children[cur_idx];
      cur_tab->move_relative(*this, 2, 1);
      cur_tab->resize(rect.h - 2, rect.w);
      cur_tab->visible = true;
      cur_tab->arrange();
    }

    button::button(std::function<void()> c) : widget() {
      callback = c;
    }

    void button::feed(set<InterfaceKey>& events) {
      if (enabler.tracking_on) {
        int32_t mx = 0, my = 0;
        gps.get_mouse_text_coords(mx, my);
        if (inside(my, mx)) {
          callback();
        }
      }
    }

    better_button::better_button() : widget() {
      display_string = NULL;
      texpos = NULL;
      callback = NULL;
      check_truth = NULL;
    }

// If you use this constructor with any bool that doesn't have a static lifetime it will not be pretty
    better_button::better_button(bool* b) : widget() {
      display_string = [b]() {
        if (*b) {
          return "Yes";
        }
        else {
          return "No";
        }
      };
      texpos = []() {
        return (int32_t*) & (init.texpos_confirm_intro_button);
      };
      callback = [b]() {
        *b = !*b;
      };
      check_truth = [b]() {
        return *b;
      };

    }

    void better_button::feed(std::set<InterfaceKey>& events) {
      if (enabler.tracking_on) {
        int32_t mx = 0, my = 0;
        gps.get_mouse_text_coords(mx, my);
        if (inside(my, mx) && enabler.mouse_lbut) {
          callback();
          enabler.mouse_lbut = false;
        }
      }
    }
    void better_button::render() {
      int32_t sy, sx, ey, ex;
      coords(sy, sx, ey, ex);
      int32_t* tex = texpos();
      string s = display_string();
      if (tex) draw_nineslice(tex, sy, sx, ey, ex);
      if (s.size() > 0) {
        gps.locate((sy + ey) / 2, (sx+ex)/2 - ((int32_t)s.size()/2));
        gps.changecolor(7, 0, 1);
        gps.addst(s, justify_center);
      }
    }

    textbox::textbox(int len, uint32_t flag) : widget() {
      str = "";
      maxlen = len;
      flags = flag;
      fg = 7;
      bg = 0;
      bright = 1;
      rect.w = len + 6;
      rect.h = 3;
      input = false;
      toggle = button([this]() {
        if (enabler.mouse_lbut) {
          this->input = !this->input;
          enabler.mouse_lbut = false;
        }
      });
      set_callback([](auto _) {});
      arrange();
    }

    void textbox::feed(set<InterfaceKey> &events) {
      toggle.feed(events);
      if (input) {
        if (standardstringentry(str, maxlen, flags, events)) {
          callback(this);
        }
      }
    }

    void textbox::render() {
  // We need to do some kind of line-breaking for multi-line text
  // entry boxes. This shall be implemented at need, and there is none
  // yet.
      int sy, sx, ey, ex;
      coords(sy, sx, ey, ex);
      for (auto by = sy; by <= ey; ++by)
      {
        gps.locate(by, sx);
        gps.add_lower_tile(::init.texpos_button_filter[0][by - sy]);
        gps.locate(by, ex - 3);
        gps.add_lower_tile(::init.texpos_button_filter_name[0][by - sy]);
        gps.locate(by, ex - 2);
        gps.add_lower_tile(::init.texpos_button_filter_name[1][by - sy]);
        gps.locate(by, ex - 1);
        gps.add_lower_tile(::init.texpos_button_filter_name[2][by - sy]);
        gps.locate(by, ex);
        gps.add_lower_tile(::init.texpos_button_filter_name[3][by - sy]);
        for (auto bx = sx + 1; bx <= ex - 4; ++bx)
        {
            //text
          gps.locate(by, bx);
          gps.add_lower_tile(::init.texpos_button_filter[1][by - sy]);
            if (init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))gps.addchar(0);//clear out drawborder
          }
        }

        gps.locate(sy + 1, sx + 2);
        gps.changecolor(fg, bg, bright);
        string display_str = str;
        if (input && GetTickCount() % 1000 < 500)display_str += "_";
        gps.addst(display_str);
        return;
      }

      text::text(string& s) {
        fg = 7;
        bg = 0;
        bright = 1;
        set_text(s);
      }

      void text::render() {
        gps.locate(rect.y, rect.x);
        gps.changecolor(fg, bg, bright);
        gps.addst(str);
      }

      void nineslice::render() {
        int32_t sy, sx, ey, ex;
        coords(sy, sx, ey, ex);
        draw_nineslice(selected_texpos, sy, sx, ey, ex);
      }

      dropdown::dropdown() : widget() {
        cur_selected = 0;
        rect.h = 3;
    set_callback([](auto& s) {}); // no-op, not sure if null std::functions crash
  }

  dropdown::dropdown(std::initializer_list<string> new_options) : widget() {
    cur_selected = 0;
    rect.h = 3;
    std::copy(new_options.begin(), new_options.end(), options.begin());
    for (auto& s : options) {
      rect.w = max(rect.w, (int32_t)s.length() + 4);
    }
    set_callback([](auto& s) {});
  }

  void dropdown::add_option(std::string& s) {
    options.push_back(s);
    rect.w = max(rect.w, (int32_t)s.length() + 4);
  }

  void dropdown::feed(set<InterfaceKey>& events) {
    // TODO TODO DON'T FORGET THIS WOW
  }

  void dropdown::render() {
    int32_t sx, ex, sy, ey;
    coords(sy, sx, ey, ex);
    //draw what we have
    gps.locate(sy + 1, sx);
    gps.addst(options[cur_selected]);
    // draw the button
    draw_nineslice(init.texpos_open_list_button, sy, ex - 2, ey, ex);
    if (open) {
      int by = sy;
      for (int i = 0; i < options.size(); i++) {
        if (i == cur_selected) {
          draw_nineslice(init.texpos_selected_intro_button, by, sx + 1, by + 2, ex);
        }
        else {
          draw_nineslice(init.texpos_unselected_intro_button, by, sx + 1, by + 2, ex);
        }
        gps.locate(by + 1, ex - (int32_t)options[i].size() - 1);
        gps.addst(options[i]);
        by += 3;
      }
    }
  }