#ifndef VIEWBASE_H
#define VIEWBASE_H

#include <set>
#include <map>
#include <string>
#include <functional>
#include <algorithm>

#include "../util/svector.hpp"
#include "../graphics/keybindings.hpp"
#include "../graphics/graphics.hpp"

struct world_gen_param_valuest;

#define SCROLLBAR_DISPLAY_FLAG_BASIC_SET BIT1

void draw_nineslice(int32_t* slice, int sy, int sx, int ey, int ex, bool override=false);

struct scrollbarst
{
  int32_t sel;
  int32_t sel_min;
  int32_t sel_max;
  int32_t page_size;
  int32_t print_sy;
  int32_t print_ey;

  int32_t scroller_sy;
  int32_t scroller_ey;


  scrollbarst()
  {
    sel = 0;
    sel_min = 0;
    sel_max = 0;
    print_sy = 0;
    print_ey = 0;
    scroller_sy = 0;
    scroller_ey = 0;
  }

  void init(int32_t n_sel, int32_t n_smn, int32_t n_smx, int32_t npnm, int32_t psy, int32_t pey)
  {
    sel = n_sel;
    sel_min = n_smn;
    sel_max = n_smx;
    resize(npnm, psy, pey);

    set_scroller_start_end();
  }

  void resize(int32_t npnm, int32_t psy, int32_t pey) {
    print_sy = psy;
    print_ey = pey;
    page_size = npnm;
  }

  void set_select_on_mouse_y(int32_t mouse_y)
  {
    if (mouse_y <= print_sy)sel = sel_min;
    else if (mouse_y >= print_ey)
    {
      sel = sel_max - page_size + 1;
      if (sel < sel_min)sel = sel_min;
    }
    else if (print_ey > print_sy)
    {
      int32_t range = sel_max - sel_min + 1 - page_size;
      if (range < 0)range = 0;
      sel = (mouse_y - print_sy) * range / (print_ey - print_sy + 1) + sel_min;
      if (sel > sel_max - page_size + 1)sel = sel_max - page_size + 1;
      if (sel < sel_min)sel = sel_min;
    }

    set_scroller_start_end();
  }

  void set_scroller_start_end()
  {
    int32_t range = sel_max - sel_min + 1;
    if (range <= 0)return;
    int32_t width = (page_size - 1) * (print_ey - print_sy + 1) / range;
    if (width < 1)width = 1;
    int32_t cur_min = sel;

    scroller_sy = cur_min * (print_ey - print_sy + 1) / range + print_sy;
    scroller_ey = scroller_sy + width;
        //place scrollers at end if they are at end
    if (sel == sel_min)
    {
      scroller_sy = print_sy;
      scroller_ey = scroller_sy + width;
    }
    if (sel == sel_max - page_size + 1)
    {
      scroller_ey = print_ey;
      scroller_sy = scroller_ey - width;
    }
        //bring it in one if you aren't at the end of the bar either way
    if (scroller_sy <= print_sy && sel > sel_min)
    {
      scroller_sy = print_sy + 1;
      scroller_ey = scroller_sy + width;
    }
    if (scroller_ey >= print_ey && sel < sel_max - page_size + 1)
    {
      scroller_ey = print_ey - 1;
      scroller_sy = scroller_ey - width;
    }
        //clamp
    if (scroller_sy < print_sy)
    {
      scroller_sy = print_sy;
      scroller_ey = scroller_sy + width;
    }
    if (scroller_sy > print_ey)
    {
      scroller_sy = print_ey;
      scroller_ey = scroller_sy + width;
    }
    if (scroller_ey < print_sy)
    {
      scroller_ey = print_sy;
      scroller_sy = scroller_ey - width;
    }
    if (scroller_ey > print_ey)
    {
      scroller_ey = print_ey;
      scroller_sy = scroller_ey - width;
    }
        //handle collapsed scroller
    if (scroller_sy == scroller_ey)
    {
      if (scroller_sy < print_ey)
      {
        scroller_ey = scroller_sy + 1;
        scroller_sy = scroller_ey - width;
        if (scroller_ey >= print_ey && sel < sel_max - page_size + 1)
        {
          --scroller_sy;
          --scroller_ey;
                    if (scroller_sy < print_sy)scroller_sy = print_sy;//only happens if scrollbar is height <=2
                    scroller_ey = scroller_sy + width;
                  }
                }
                else
                {
                  scroller_sy = scroller_ey - 1;
                if (scroller_sy < print_sy)scroller_sy = print_sy;//only happens if scrollbar is height 1...  or less
                scroller_ey = scroller_sy + width;
              }
            }
        //final clamp, disregarding everything
            if (scroller_sy < print_sy)scroller_sy = print_sy;
            if (scroller_sy > print_ey)scroller_sy = print_ey;
            if (scroller_ey < print_sy)scroller_ey = print_sy;
            if (scroller_ey > print_ey)scroller_ey = print_ey;
          }

          void render(int32_t mouse_x, int32_t mouse_y, int32_t bx, bool active, uint32_t display_flag = 0);
          void handle_initial_click(int32_t mouse_y, int32_t& scroll_position, bool& scrolling);
          bool handle_events(std::set<InterfaceKey>& events, int32_t& scroll_position, bool& scrolling);
        };

        class abstract_interfacest
        {
        public:
          virtual void feed(std::set<InterfaceKey> &events) = 0;
          virtual void logic() = 0;
          virtual void render() = 0;
        };

        struct extentst {
    int32_t x, y; // sx, sy
    int32_t w, h;
    extentst() {
      x = y = -1;
      w = h = 0;
    }
  };

  namespace widgets {

    using namespace std;

    // BASE WIDGET CLASS
    class widget : public abstract_interfacest
    {
    protected:
        abstract_interfacest* parent; // will be a widget unless it's the top-level viewscreen widget
      public:
        string name;
        extentst rect;
        int32_t rel_x, rel_y;
        bool visible;
        widget();
        void set_extents(int sy, int ey, int sx, int ex);
        void resize(int h, int w);
        void move(int y, int x);
        void move_relative(widget& other);
        void move_relative(widget& other, int y, int x);
        void coords(int32_t& sy, int32_t& sx, int32_t& ey, int32_t& ex);
        bool inside(int32_t y, int32_t x);
        void set_parent(abstract_interfacest* p) { parent = p; }
        virtual void feed(std::set<InterfaceKey>& events) {}
        virtual void logic() {}
        virtual void render() {}
        virtual void arrange() {}
      };

    //CONTAINER WIDGETS
      
    // Generic container widget, no custom arrangement etc logic. Every viewscreen has one of these to make it easier to add widgets to them
      class container : public widget
      {
      public:
        svector<std::shared_ptr<widget>> children;

        template<class T, class... Args>
        std::shared_ptr<T> add_widget(Args... args)
        {
          std::shared_ptr<T> ret = std::make_shared<T>(args...);
          ret->set_parent(this);
          children.push_back(ret);
          arrange();
          return ret;
        }

        virtual void feed(std::set<InterfaceKey>& events);
        virtual void logic();
        virtual void render();
        virtual void arrange();
        ~container() {
            // shared_ptr cleans itself up when there's no references, so this is fine
          children.clear();
        }
      };

    // Keeps things in neat rows, see e.g. options viewscreens
      class rows_container : public container {
      public:
        int32_t padding;
        virtual void arrange();
      };

    // Keeps things in columns--not sure anything in vanilla is like this, no arrangement from the right or anything right now
      class columns_container : public container {
      public:
        int32_t padding;
        virtual void arrange();
      };




    // Shows only one of its children at a time, determined by cur_tab
      class tabs : public container {
        size_t cur_idx;
        svector<string> tab_labels;
      public:
        size_t get_tab() { return cur_idx; }
        string& get_cur_tab_string() { return tab_labels[cur_idx]; }
        string& get_tab_string(size_t which) { return tab_labels.at(which); }
        bool switch_tab(size_t which);
        virtual void feed(std::set<InterfaceKey>& events);
        virtual void render();
        virtual void arrange();
        void add_tab_label(const char* s);
        void add_tab_label(string& s);
      };

      class scroll_rows : public rows_container {
        int32_t scroll;
        int32_t num_visible;
        bool scrolling;
        scrollbarst scrollbar;
      public:
        virtual void feed(set<InterfaceKey>& events);
        virtual void render();
        virtual void arrange();
      };
/* YAGNI for now
    class scroll_columns : public columns_container {
        int32_t scroll;
        virtual void render();
        virtual void arrange();
    };
*/

    // DISPLAY WIDGETS

    // Just displays a string where it's put, nothing special--only here to allow text to conform to the widget system
      class text : public widget {
        string str;
      public:
        virtual void render();
        short fg, bg;
        char bright;
        text(string& s);
        void set_text(string& s) {
          str = s;
          resize((int32_t)str.length(), 1);
        }
        string get_text() {
          return str;
        }
      };

    // Specialized for options lists
      class params_container : public container {
        std::shared_ptr<text> tx;
        std::shared_ptr<::world_gen_param_valuest> param;
        void update_text(string& str) {
          if (tx) tx->set_text(str);
        }
      public:
        params_container(std::shared_ptr<::world_gen_param_valuest> param);
      };


    // Give it one of the nineslice texboxes and it'll automagically draw it in its extents. Science?
      class nineslice : public widget {
        int32_t* selected_texpos;
      public:
        virtual void render();
        nineslice(int32_t* which) {
          set_texpos(which);
        }
        void set_texpos(int32_t* which) {
          selected_texpos = which;
        }
      };

    //INPUT WIDGETS

    // BUTTON
    // Actually checking if the mouse button and capturing such clicks is up to the callback, as is behavior
      class button : public widget {
        std::function<void()> callback;
      public:
        button() : widget() { callback = 0; };
        button(std::function<void()> c);
        virtual void feed(std::set<InterfaceKey>& events);
        virtual void render() {} // this is an "abstract button", it doesn't render--up to whatever's using it to do that, this just have the click behavior
        virtual void logic() {}
      };

    // BETTER BUTTON
    // The button's cooler younger sibling. Learned to render and display text on its own. Requires a lot of functions, though.
      class better_button : public widget {
        std::function<string()> display_string;
        std::function<int32_t*()> texpos;
        std::function<void()> callback;
        std::function<bool()> check_truth;
      public:
        better_button();
        better_button(bool* b);
        void set_callback(std::function<void()> f) {
          callback = f;
        }
        void set_check(std::function<bool()> f) {
          check_truth = f;
        }
        void set_textures(std::function<int32_t*()> f) {
          texpos = f;
        }
        void set_strings(std::function<string()> f) {
          display_string = f;
        }
        bool get() { return check_truth(); }
        virtual void feed(std::set<InterfaceKey>& events);
        virtual void render();
      };

      class textbox : public widget {
        string str;
        button toggle;
        uint32_t flags; // STRINGENTRY_LETTERS etc
        std::function<void(textbox*)> callback;
      public:
        textbox(int len, uint32_t flag);
        short fg, bg;
        char bright;
        bool input;
        int maxlen;
        string get_text() { return str; }
        void set_string(string& s) { str = s; }
        void set_callback(std::function<void(textbox*)> f) { callback = f; }
        virtual void feed(std::set<InterfaceKey>& events);
        virtual void render();
        virtual void arrange() { toggle.set_extents(rect.y, rect.y + rect.h, rect.x + rect.w - 4, rect.x + rect.w); }

      };

      class dropdown : public widget {
        size_t cur_selected;
        svector<string> options;
        std::function<void(string&)> callback;
        bool open;
      public:
        dropdown();
        dropdown(std::initializer_list<string> s);
        virtual void feed(std::set<InterfaceKey>& events);
        virtual void render();
        void set_callback(std::function<void(string&)>c) { callback = c; };
        void add_option(std::string& s);
      };

    template <typename T>
      class menu {
        typedef map<int, pair<string, T> > dict;
        dict lines;
        int selection;
        int last_displayheight;
        bool bleached;
        map<int, pair<int, int> > colors;

        // Given 'total' lines, with 'sel' selected, and 'space' to draw in,
        // returns the first line that should be drawn.
        int first_line(int total, int sel, int space) {
            // There is no doubt some clever math to do this, but I'm tired and don't care.
          for (int start = 0;; start += space / 2) {
            if (start + space / 2 >= sel) return start;
            if (start + space >= total) return start;
          }
        }
        pair<string, T> mp(string s, T t) { return make_pair(s, t); }

        // Scrolls N lines up/down; positive = down
        void scroll(int n) {
          typename dict::iterator it = lines.find(selection);
          for (int i = 0; i < abs(n); i++) {
                if (n < 0 && it == lines.begin()) { // We've hit the top
                  if (i) break;
                  else {
                    it = --(lines.end());
                    break;
                  }
                }
                if (n < 0) --it; else ++it; // Scroll one line
                if (it == lines.end()) { // We've hit the bottom
                  if (i) {
                    --it;
                    break;
                  }
                  else {
                    it = lines.begin();
                    break;
                  }
                }
                // If we hit neither the top nor bottom, loop.
              }

              selection = it->first;
            }

          public:
            menu() { clear(); }
            int size() { return lines.size(); }
        // Adds a line just past the last taken position
            void add(string text, T token) {
              if (!lines.size()) {
                lines[0] = mp(text, token);
              }
              else {
                typename dict::iterator it = --(lines.end());
                lines[it->first + 1] = mp(text, token);
              }
            }
        // (Re)sets the text of the given line
            void set(int line, string text, T token) {
              lines[line] = mp(text, token);
            }
        // Set the color of a line
            void set_color(int line, int fg, int bg) {
              colors[line] = make_pair(fg, bg);
            }
        // Handles (page) up/down
            void feed(std::set<InterfaceKey>& input) {
              if (!lines.size()) return;
              if (input.count(INTERFACEKEY_STANDARDSCROLL_UP)) scroll(-1);
              if (input.count(INTERFACEKEY_STANDARDSCROLL_DOWN)) scroll(1);
              if (input.count(INTERFACEKEY_STANDARDSCROLL_PAGEUP)) scroll(-(last_displayheight / 2));
              if (input.count(INTERFACEKEY_STANDARDSCROLL_PAGEDOWN)) scroll(last_displayheight / 2);
            }
            void render(int x1, int x2, int y1, int y2) {
              gps.erasescreen_rect(x1, x2, y1, y2);
              int h = y2 - y1 + 1,
              w = x2 - x1 + 1,
              x = x1, y = y1;
              last_displayheight = h;
              if (!lines.size()) return;
              int total = (--lines.end())->first + 1;
              int first = first_line(total, selection, h);
              typename dict::iterator it = lines.lower_bound(first);
              for (; it != lines.end() && it->first - first < h; ++it) {
                gps.locate(it->first - first + y, x);
                map<int, pair<int, int> >::iterator color = colors.find(it->first - first);
                int fg = 7, bg = 0;
                if (color != colors.end()) {
                  fg = color->second.first;
                  bg = color->second.second;
                }
                gps.changecolor(fg, bg, it->first == selection && !bleached);
                gps.addst(it->second.first.substr(0, w));
              }
            }
        // Read out the current selection
            T get_selection() { return lines[selection].second; }
            int get_pos() { return selection; }
        // Set the position by line
            void set_pos(int pos) {
              if (pos < size())
                selection = pos;
            }
        // Delete the currently selected line
            void del_selection() {
              typename dict::iterator it = lines.find(selection);
              typename dict::iterator newsel = it;
              ++newsel;
              if (newsel == lines.end()) {
                newsel = it;
                --newsel;
              }
              lines.erase(it);
              if (lines.size()) selection = newsel->first;
            }
        // If true, don't draw a highlight
            void bleach(bool b) { bleached = b; }
        // Reset the menu
            void clear() {
              selection = 0;
              lines.clear();
              last_displayheight = 10;
              bleached = false;
              colors.clear();
            }
          };

        }

        enum InterfaceBreakdownTypes
        {
          INTERFACE_BREAKDOWN_NONE,
          INTERFACE_BREAKDOWN_QUIT,
          INTERFACE_BREAKDOWN_STOPSCREEN,
          INTERFACE_BREAKDOWN_TOFIRST,
          INTERFACE_BREAKDOWNNUM
        };

        class viewscreenst : abstract_interfacest
        {
        public:
          viewscreenst *child;
          viewscreenst *parent;
          char breakdownlevel;

          char option_key_pressed;
          widgets::container widgets;
          virtual void feed(std::set<InterfaceKey>& events) { widgets.feed(events); }
          virtual void logic() { widgets.logic(); }
          virtual void render() { widgets.render(); }
          virtual void resize(int w, int h){}

#ifdef CURSES_MOVIES
          virtual char movies_okay(){return 1;}
#endif

          virtual void set_port_flags();

          viewscreenst();
          virtual ~viewscreenst(){}
        };

#endif
