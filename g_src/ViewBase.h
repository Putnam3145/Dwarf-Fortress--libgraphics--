#ifndef VIEWBASE_H
#define VIEWBASE_H

#include <set>
#include <map>
#include <string>
#include <functional>
#include <algorithm>
#include <unordered_set>
#include <optional>

#include "svector.h"
#include "keybindings.h"
#include "graphics.h"
#include "basics.h"

struct world_gen_param_valuest;

#define SCROLLBAR_DISPLAY_FLAG_BASIC_SET BIT1

// Helper drawing functions
void draw_nineslice(int32_t* slice, int sy, int sx, int ey, int ex, override_tile_type override=0);

void draw_horizontal_nineslice(int32_t *slice,int sy,int sx,int ey,int ex,override_tile_type override=0);

void draw_sort_widget(int sy,int sx,int ey,int ex,bool active,bool ascending);

void draw_sort_widget(int sy,int sx,int ey,int ex,bool active,bool ascending,const string &str);

void blank_anchors(int32_t sy,int32_t sx,int32_t ey,int32_t ex);

// Helper input functions
bool standardscroll(std::set<InterfaceKey> &events, int32_t &scroll, int32_t page_size);

enum struct TooltipType : int8_t
    {
    REPLACE_MINIMAP,
    ABOVE,
    BELOW,
    NONE=-1,
    };

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

    void set_select_on_mouse_y(int32_t mouse_y);

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

struct extentst {
    int32_t sx,sy,ex,ey;
    extentst() {
        sx=sy=ex=ey=0;
    }
    int32_t w() { return ex-sx+1; }
    int32_t h() { return ey-sy+1; }
};


class abstract_interfacest
{
public:
    virtual extentst get_rect() { return extentst(); }
    virtual void feed(std::set<InterfaceKey> &events) = 0;
    virtual void logic() = 0;
    virtual void render(uint32_t curtick=0) = 0;
    virtual void arrange() {}
    virtual void remove_child(void *w)=0;
    virtual void clear()=0;
};

namespace widgets {

    // "Visible" is what containers should be using for contained widgets and what
    // widgets themselves should be using--"active" should be used for 
    // stuff like folders or filters that externally change whether such things
    // are visible
    using WidgetFlag=uint8_t;

    constexpr WidgetFlag WIDGET_VISIBILITY_ACTIVE=BIT1;
    constexpr WidgetFlag WIDGET_VISIBILITY_VISIBLE=BIT2;
    constexpr WidgetFlag WIDGET_VISIBILITY_ACTUALLY_VISIBLE=WIDGET_VISIBILITY_ACTIVE|WIDGET_VISIBILITY_VISIBLE;
    constexpr WidgetFlag WIDGET_CAN_KEY_ACTIVATE=BIT3;
    constexpr WidgetFlag WIDGET_GLOBAL_POSITIONING=BIT4;

    enum struct Side : int8_t
        {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT
        };

    enum struct LayoutPreset : int8_t
        {
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT,
        CENTER,
        CENTER_TOP,
        CENTER_BOTTOM,
        CENTER_LEFT,
        CENTER_RIGHT,
        WIDE_TOP,
        WIDE_BOTTOM,
        WIDE_LEFT,
        WIDE_RIGHT,
        FULL,
        HALF_LEFT,
        HALF_RIGHT,
        HALF_TOP,
        HALF_BOTTOM
        };

    // BASE WIDGET CLASS
    class widget : public abstract_interfacest
    {
    protected:
        abstract_interfacest* parent=NULL; // will be a widget unless it's the top-level viewscreen widget
        extentst rect;
        std::vector<std::function<void(std::set<InterfaceKey> &, widget *)>> custom_feed;
        std::vector<std::function<void(widget *)>> custom_logic;
        std::vector<std::function<void(widget *, uint32_t curtick)>> custom_render;
        std::vector<std::function<bool(widget *)>> custom_activated;
    public:
    public:
        string name;
        WidgetFlag flag=0;
        // I ripped most of this stuff off from Godot's Control
        // In general tried to make it work like https://docs.godotengine.org/en/stable/tutorials/ui/size_and_anchors.html
        // Mostly for my own benefit, haha
        // -Putnam
        int32_t offset_bottom=0;
        int32_t offset_left=0;
        int32_t offset_right=0;
        int32_t offset_top=0;
        float anchor_top=0.0;
        float anchor_bottom=0.0;
        float anchor_left=0.0;
        float anchor_right=0.0;
        int32_t min_w=0,min_h=0;
        std::variant<string, std::function<void()>> tooltip;
        TooltipType tooltip_type=TooltipType::NONE;
        bool displaying_tooltip=false;
        string search_string;
        std::set<InterfaceKey> activation_hotkeys;
        widget();
        virtual ~widget();
        int width();
        int height();
        void resize(int h, int w);
        void coords(int32_t &sy,int32_t &sx,int32_t &ey,int32_t &ex);
        void move_to_anchor();
        void set_anchor(Side side,float n);
        void set_anchors(float t,float b,float l,float r);
        void set_anchors_preset(LayoutPreset preset);
        void set_offset(Side side,int n);
        void set_offsets(int t,int b,int l,int r);
        void set_layout_preset(LayoutPreset preset);
        bool inside(int32_t y, int32_t x);
        bool set_active(bool n);
        bool set_visible(bool n);
        bool set_global_positioning(bool n);
        bool set_can_key_activate(bool n, bool children_too=true);
        bool is_visible() { return (flag&WIDGET_VISIBILITY_ACTUALLY_VISIBLE)==WIDGET_VISIBILITY_ACTUALLY_VISIBLE; }
        void set_parent(abstract_interfacest* p) { parent = p; }
        void set_custom_feed(std::function<void(std::set<InterfaceKey>&,widget *)> f) { custom_feed.push_back(f); }
        void set_custom_logic(std::function<void(widget *)> f) { custom_logic.push_back(f); }
        void set_custom_render(std::function<void(widget *)> f) { custom_render.push_back([f](widget *w,uint32_t curtick) {f(w);}); }
        void set_custom_render(std::function<void(widget *,uint32_t)> f) { custom_render.push_back(f); }
        void set_custom_activated(std::function<bool(widget *)> f) { custom_activated.push_back(f); }
        bool can_key_activate();
        bool activate();
        void locate(int32_t offset_y=0, int32_t offset_x=0);
        virtual extentst get_rect() { return rect; } // copies on purpose
        virtual void remove_child(void*) {}
        virtual void feed(std::set<InterfaceKey> &events);
        virtual void logic();
        virtual void render(uint32_t curtick=0);
        virtual void arrange() { move_to_anchor(); }
        virtual void clear();
    };

    // DISPLAY WIDGETS

    class character : public widget
        {
    protected:
        char c;
    public:
        virtual void render(uint32_t curtick=0);
        short fg,bg;
        char bright;
        uint32_t flag;
        character(char c) : c(c) {}
        };

    // Just displays a string where it's put, nothing special--only here to allow text to conform to the widget system
    class text : public widget {
    protected:
        string str;
    public:
        virtual void render(uint32_t curtick=0);
        short fg, bg;
        char bright;
        justification just=justify_left;
        int space=0;
        uint32_t flag=0;
        text() { text(""); }
        text(const string& s);
        text(string &&s);
        virtual void set_text(const string& s) {
            str = s;
            resize(1, (int32_t)str.length());
        }
        virtual void set_text(string &&s)
            {
            str=s;
            resize(1,(int32_t)str.length());
            }
        string get_text() {
            return str;
        }
        void set_color(short f,short b,char br) {
            fg=f;
            bg=b;
            bright=br;
            }
    };

    const static int32_t TRUNCATE_MODE_ABBREVIATE=BIT1;
    const static int32_t TRUNCATE_MODE_ELLIPSES=BIT2;

    // As text, but treats its min width as a max width, with various options to reduce the string's length
    class text_truncated : public text
        {
    protected:
        string original_str;
    public:
        int32_t truncate_mode_flags=0;
        virtual void arrange();
        virtual void set_text(const string &s)
            {
            str=s;
            original_str=str;
            }
        virtual void set_text(string &&s)
            {
            str=s;
            original_str=str;
            }
        size_t max_length() { return original_str.size(); }
        text_truncated() : text() {}
        text_truncated(const string &s) : text(s) { original_str=s; }
        text_truncated(string &&s) : text(s) { original_str=s; }
        };

    // As text, but treats its min width as a max width, and will insert line breaks to fit
    class text_multiline : public text
        {
        inline static svector<string> strs;
    public:
        virtual void render(uint32_t curtick=0);
        virtual void arrange();
        virtual void set_text(const string &s)
            {
            str=s;
            }
        virtual void set_text(string &&s)
            {
            str=s;
            }
        text_multiline() : text() {}
        text_multiline(const string &s) : text(s) {}
        text_multiline(string &&s) : text(s) {}
        };

    // Give it one of the nineslice texboxes and it'll automagically draw it in its extents. Science?
    class nineslice : public widget {
    protected:
        int32_t *selected_texpos;
    public:
        override_tile_type flag=OVERRIDE_CHAR;
        virtual void render(uint32_t curtick=0);
        nineslice(int32_t* which) {
            set_texpos(which);
        }
        void set_texpos(int32_t* which) {
            selected_texpos = which;
        }
        nineslice() {
            selected_texpos = NULL;
        }
    };

    // Same as nineslice, but for "horizontal" texposes that are loaded in rotated 90 degree
    class nineslice_horizontal : public nineslice {
    public:
        nineslice_horizontal(int32_t *which) : nineslice(which) {}
        virtual void render(uint32_t curtick=0);
        };

    // Just a function instead of a widget because it's intended to be used with custom_render
    // Mostly because it's often used with tables which don't work well here
    void display_picture_box(widget *w);

    // Will print an anchored tile at the location
    class anchored_tile : public widget
        {
        long texp,offset_x,offset_y;
        bool use_color;
    public:
        virtual void render(uint32_t curtick=0);
        anchored_tile(long texp, long offset_x, long offset_y, bool use_color) :
            texp(texp),
            offset_x(offset_x),
            offset_y(offset_y),
            use_color(use_color)
            {}
        };

    class keybinding_display : public widget {
        int binding;
    public:
        keybinding_display(int binding) : binding(binding) {}
        virtual void arrange();
        virtual void render(uint32_t curtick=0);
        };

    class graphics_switcher : public widget
        {
        std::shared_ptr<widget> graphics_widget;
        std::shared_ptr<widget> ascii_widget;
    public:
        template<class T, class... Args>
        std::shared_ptr<T> add_ascii_widget(Args... args)
            {
            auto w=std::make_shared<T>(args...);
            ascii_widget=w;
            w->set_parent(this);
            return w;
            }
        template<class T, class... Args>
        std::shared_ptr<T> add_graphics_widget(Args... args)
            {
            auto w=std::make_shared<T>(args...);
            graphics_widget=w;
            w->set_parent(this);
            return w;
            }
        std::shared_ptr<widget> current_widget();
        virtual void render(uint32_t curtick=0) { current_widget()->render(curtick); }
        virtual void logic() { current_widget()->logic(); }
        virtual void arrange() { current_widget()->arrange(); }
        virtual void feed(std::set<InterfaceKey> &events) { current_widget()->feed(events); }
        };

    //CONTAINER WIDGETS
    
    // Generic container widget, no custom arrangement etc logic. Every viewscreen has one of these to make it easier to add widgets to them
    class container : public widget
    {
    private:
        std::map<std::string,std::shared_ptr<widget>> children_by_name;
    public:
        svector<std::shared_ptr<widget>> children;
        template<class T, class... Args>
        std::shared_ptr<T> add_or_get_widget(const std::string &s,Args... args)
            {
            if (auto it=children_by_name.find(s); it!=children_by_name.end())
                {
                return std::dynamic_pointer_cast<T>(it->second);
                }
            auto r=std::make_shared<T>(args...);
            r->name=s;
            r->set_parent(this);
            children.push_back(r);
            children_by_name.insert({ s,r });
            arrange();
            return r;
            }
        template<class T>
        std::shared_ptr<T> add_or_get_widget(const std::string &s,std::shared_ptr<T> widget) {
            widget->name=s;
            widget->set_parent(this);
            if (auto it=children_by_name.find(s); it!=children_by_name.end())
                {
                it->second=widget;
                for (auto &child : children)
                    {
                    if (child->name == s)
                        {
                        child=widget;
                        }
                    }
                return widget;
                }
            children.push_back(widget);
            children_by_name.insert({s, widget});
            arrange();
            return widget;
            }
        std::shared_ptr<widget> get_by_name(const std::string &s);
        template<class T, class... Args>
        std::shared_ptr<T> add_widget(Args... args)
        {
            std::shared_ptr<T> ret = std::make_shared<T>(args...);
            ret->set_parent(this);
            children.push_back(ret);
            arrange();
            return ret;
        }
        template<class T>
        std::shared_ptr<T> add_widget(std::shared_ptr<T> widget)
            {
            widget->set_parent(this);
            children.push_back(widget);
            arrange();
            return widget;
            }
        bool has_widget_name(const std::string &n) {
            return children_by_name.contains(n);
            }
        bool has_widget_name(std::string &&n) {
            return children_by_name.contains(n);
            }
        int visible_children();
        int active_children();
        virtual void feed(std::set<InterfaceKey>& events);
        virtual void logic();
        virtual void render(uint32_t curtick=0);
        virtual void arrange();
        virtual ~container()
            {
            // shared_ptr cleans itself up when there's no references, so this is fine
            clear();
            }
        virtual void remove_child(const string &id);
        virtual void remove_child(string &&id);
        virtual void remove_child(void *w);
        virtual void clear()
            {
            children.clear();
            children_by_name.clear();
            }
    };

    #define WIDGET_STACK_DEFER_FLAG_POP BIT1
    #define WIDGET_STACK_DEFER_FLAG_BREAK_DOWN BIT2

    // As container, but simply displays the last of its children, rather than all.
    class widget_stack : public container
        {
        uint8_t defer_flags=0;
        std::shared_ptr<widget> deferred_replacement=NULL;
    public:
        void pop() { defer_flags=WIDGET_STACK_DEFER_FLAG_POP; }
        void pop_and_close() { defer_flags=(WIDGET_STACK_DEFER_FLAG_POP|WIDGET_STACK_DEFER_FLAG_BREAK_DOWN); }
        void deferred_replace(std::shared_ptr<widget> widget);
        void do_replacements();
        virtual void arrange();
        virtual void render(uint32_t curtick=0);
        virtual void feed(std::set<InterfaceKey> &events);
        virtual void logic() { if (children.size()) children.back()->logic(); }
        };

    // Keeps things in neat rows, see e.g. options viewscreens
    class rows_container : public container {
    public:
        int32_t padding=0;
        virtual void arrange();
    };

    // Keeps things in columns
    class columns_container : public container {
    public:
        int32_t padding=0;
        virtual void arrange();
    };

    // Shows only one of its children at a time, determined by cur_tab
    class tabs : public container {
        size_t cur_idx;
        svector<string> tab_labels;
        std::vector<std::pair<size_t,size_t>> rows;
    public:
        enum struct TabType
            {
            DEFAULT,
            SHORT,
            SHORT_SUB,
            SHORT_SUBSUB
            };
        TabType tab_type=TabType::DEFAULT;
        size_t get_tab() { return cur_idx; }
        string get_cur_tab_string() { if (tab_labels.size()==0) { return ""; } else { return tab_labels[cur_idx]; } }
        string get_tab_string(size_t which) { return tab_labels.at(which); }
        bool switch_tab(size_t which);
        bool switch_tab(const string &s);
        virtual void feed(std::set<InterfaceKey>& events);
        virtual void render(uint32_t curtick=0);
        virtual void arrange();
        virtual void clear();
        void add_tab_label(const char* s);
        void add_tab_label(const string& s);
        template<class T,class... Args>
        std::shared_ptr<T> add_tab(const char* s,Args... args) {
            if(!has_widget_name(s)) add_tab_label(s);
            return add_or_get_widget<T>(s,args...);
            }
        template<class T,class... Args>
        std::shared_ptr<T> add_tab(const string &s,Args... args) {
            if(!has_widget_name(s)) add_tab_label(s);
            return add_or_get_widget<T>(s,args...);
            }
    };

    class scroll_rows : public rows_container {
        int32_t scroll;
        int32_t num_visible;
        bool scrolling;
        scrollbarst scrollbar;
        int8_t scrollbar_display_flags=SCROLLBAR_DISPLAY_FLAG_BASIC_SET;
    public:
        void handle_scroll(std::set<InterfaceKey> &events);
        virtual void feed(std::set<InterfaceKey>& events);
        virtual void render(uint32_t curtick=0);
        virtual void arrange();
        void scroll_to_bottom();
        void scroll_to_top();
        void page_down();
        void page_up();
        int32_t page_size();
        void scroll_to(int32_t idx);
    };

    class radio_rows : public widget {
        widget *selected;
        int32_t selected_idx=0;
        scroll_rows rows;
        std::map<size_t, std::function<void(widget *)>> select_callback;
    public:
        std::shared_ptr<container> add_entry(const std::string &,std::function<void(widget *)>);
        void rename_entry(const std::string &,const std::string &);
        void set_selected(int32_t idx);
        void set_selected(void *widget);
        void refresh_selected();
        radio_rows();
        virtual void feed(std::set<InterfaceKey> &events);
        virtual void render(uint32_t curtick=0) { widget::render(curtick); rows.render(curtick); }
        virtual void logic() { widget::logic(); rows.logic(); }
        virtual void arrange() { widget::arrange(); rows.arrange(); }
    };

    class table : public container {
    public:
        std::shared_ptr<columns_container> labels;
        std::shared_ptr<scroll_rows> entries;
        std::shared_ptr<widget> key_display;
        int32_t key_row,key_col;
        bool keyboard_controlled=true;
        table();
        virtual void arrange();
        virtual void feed(std::set<InterfaceKey> &events);
        template<class T,class... Args> std::shared_ptr<T> add_row(Args... args) { return entries->add_widget<T>(args...); }
        template<class T,class... Args> std::shared_ptr<T> add_label(Args... args) { return labels->add_widget<T>(args...); }
    };

/* YAGNI for now
    class scroll_columns : public columns_container {
        int32_t scroll;
        virtual void render(uint32_t curtick=0);
        virtual void arrange();
    };
*/

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

    //INPUT WIDGETS

    class interface_main_button : public widget
        {
       public:
           int32_t which_button;
           interface_main_button() : widget() { which_button=0; resize(3,4); }
           interface_main_button(int32_t w) : widget(),which_button( w ) { resize(3,4); }
           virtual void render(uint32_t curtick=0);
        };

    class interface_small_button : public widget {
    public:
        int32_t which_button;
        interface_small_button() : widget() { which_button=0; resize(2,2); }
        interface_small_button(int32_t w) : widget(),which_button(w) { resize(2,2); }
        virtual void render(uint32_t curtick=0);
        };

    struct interface_pets_livestock_button : public widget {
        int32_t which_button;
        interface_pets_livestock_button() : widget() { which_button=0; resize(3,3); }
        interface_pets_livestock_button(int32_t w) : widget(),which_button(w) { resize(3,3); }
        virtual void render(uint32_t curtick=0);
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
        virtual void render(uint32_t curtick=0);
    };

    enum struct TextboxType {
        FILTER,
        NAME,
        NONE
        };

    // TEXTBOX
    // A standard textbox with an input button widget on its right end. Callback allows for input to be manipulated and used arbitrarily.
    class textbox : public widget {
        string str;
        uint32_t flags; // STRINGENTRY_LETTERS etc
        std::function<void(textbox*)> callback;
    public:
        textbox(int len, uint32_t flag);
        short fg, bg;
        char bright;
        int maxlen;
        TextboxType textbox_type=TextboxType::FILTER;
        string get_text() { return str; }
        void set_string(string& s) { str = s; }
        void set_callback(std::function<void(textbox*)> f) { callback = f; }
        void take_focus();
        void remove_focus();
        bool is_focused();
        bool do_input(std::set<InterfaceKey> &events);
        virtual ~textbox() {
            remove_focus();
            }
        virtual void feed(std::set<InterfaceKey>& events);
        virtual void render(uint32_t curtick=0);

    };

    // DROPDOWN
    // A dropwdown list. Callback allows arbitrarily doing whatever when the dropdown entry is changed.
    class dropdown : public widget {
        size_t cur_selected;
        svector<string> options;
        std::function<void(size_t, dropdown *)> callback;
        bool open;
    public:
        dropdown();
        dropdown(std::initializer_list<string> s);
        virtual void feed(std::set<InterfaceKey>& events);
        virtual void render(uint32_t curtick=0);
        void set_callback(std::function<void(size_t, dropdown*)>c) { callback = c; };
        void add_option(std::string& s);
        void select_option(int32_t i);
        void select_option(std::string &s);
    };

    // FOLDER
    // Allows collapsing widgets into organizational units
    // Notably, does not *parent* any of the widgets it controls;
    // it's on the same level as the widgets it controls,
    // and merely deletes them from the parent when
    // they need to be hidden.
    // (It puts them back when they're to be shown)
    class folder : public widget
        {
        widget open;
        bool last_visible;
        std::unordered_set<std::shared_ptr<widget>> controlled_set;
    public:
        folder(std::shared_ptr<container>);
        bool controlled_visible;
        text label;
        std::weak_ptr<container> container_parent;
        void add_controlled(std::shared_ptr<widget> w)
            {
            controlled_set.insert(w);
            }
        virtual void arrange();
        virtual void render(uint32_t curtick=0);
        virtual void feed(std::set<InterfaceKey> &ev);
        };

    struct filter_entry
        {
        std::shared_ptr<text> label;
        std::unordered_set<std::shared_ptr<widget>> filtered_set;
        };


    // FILTER
    // Radio buttons that can filter in/out stuff.
    // Acts much like folder.
    class filter : public widget
        {
    protected:
        filter_entry all_filter;
        svector<filter_entry> filters;
        int32_t filter_idx;
    public:
        filter(std::shared_ptr<container>);
        void add_new_filter(string name);
        void add_to_filter(string name,std::shared_ptr<widget> w);
        void remove_from_filter(string name,std::shared_ptr<widget> w);
        void clear_filters(std::shared_ptr<widget> w);
        bool contains(std::shared_ptr<widget> w);
        int32_t get_filtered();
        virtual void set_filtered(int32_t idx);
        std::weak_ptr<container> container_parent;
        virtual void arrange();
        virtual void render(uint32_t curtick=0);
        virtual void feed(std::set<InterfaceKey> &ev);
        };

    class multifilter : public widget
        {
        class indiv_filter : public filter
            {
            public:
            virtual void set_filtered(int32_t idx);
            std::unordered_set<std::shared_ptr<widget>> &get_filtered_set();
            indiv_filter(std::shared_ptr<container> c) : filter(c) {}
            };
        svector<std::shared_ptr<indiv_filter>> filters;
    public:
        multifilter(std::shared_ptr<container>);
        std::shared_ptr<indiv_filter> add_new_filter_block();
        std::weak_ptr<container> container_parent;
        virtual void arrange();
        virtual void render(uint32_t curtick=0);
        virtual void feed(std::set<InterfaceKey> &ev);
        };

    template <typename T>
    class menu {
        typedef std::map<int, std::pair<string, T> > dict;
        dict lines;
        int selection;
        int last_displayheight;
        bool bleached;
        std::map<int, std::pair<int, int> > colors;

        // Given 'total' lines, with 'sel' selected, and 'space' to draw in,
        // returns the first line that should be drawn.
        int first_line(int total, int sel, int space) {
            // There is no doubt some clever math to do this, but I'm tired and don't care.
            for (int start = 0;; start += space / 2) {
                if (start + space / 2 >= sel) return start;
                if (start + space >= total) return start;
            }
        }
        std::pair<string, T> mp(string s, T t) { return make_pair(s, t); }

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
            colors[line] = std::make_pair(fg, bg);
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
                std::map<int, std::pair<int, int> >::iterator color = colors.find(it->first - first);
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

class viewscreenst : public abstract_interfacest
{
	public:
		viewscreenst *child;
		viewscreenst *parent;
		char breakdownlevel;

		char option_key_pressed;
        widgets::container widgets;
        virtual extentst get_rect();
        virtual void feed(std::set<InterfaceKey> &events);
        virtual void logic() { widgets.logic(); }
        virtual void render(uint32_t curtick=0) { widgets.render(curtick); }
        virtual void resize(int w, int h){}
        virtual void remove_child(void *w) { widgets.remove_child(w); }
        virtual void clear() { widgets.clear(); }

#ifdef CURSES_MOVIES
		virtual char movies_okay(){return 1;}
#endif

		virtual void set_port_flags();

		viewscreenst();
		virtual ~viewscreenst(){}
};

#endif
