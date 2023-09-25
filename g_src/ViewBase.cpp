#include <assert.h>
#include <iostream>
#include "ViewBase.h"
#include "interface.h"
#include "init.h"

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
	visibility_flags=WIDGET_VISIBILITY_ACTUALLY_VISIBLE;
	name = "";
	tooltip="";
	tooltip_type=TooltipType::NONE;
	set_anchors_preset(LayoutPreset::TOP_LEFT);
	set_offsets(0,0,0,0);
	min_h=min_w=0;
}

void widget::move_to_anchor() {
	// for the opinionated: the parens are for clarity, it changes no semantics, i know this
	if (!parent) parent=gview.grab_lastscreen();
	extentst parent_rect=parent->get_rect();
    rect.sx=parent_rect.sx+((parent_rect.ex-parent_rect.sx)*anchor_left)+offset_left;
	rect.ex=parent_rect.sx+((parent_rect.ex-parent_rect.sx)*anchor_right)+offset_right;
    rect.sy=parent_rect.sy+((parent_rect.ey-parent_rect.sy)*anchor_top)+offset_top;
	rect.ey=parent_rect.sy+((parent_rect.ey-parent_rect.sy)*anchor_bottom)+offset_bottom;
	if (rect.w()<min_w) rect.ex=rect.sx+min_w-1;
	if (rect.h()<min_h) rect.ey=rect.sy+min_h-1;
}

void widget::set_anchor(Side side,float n)
	{
	switch (side)
		{
		case Side::TOP:
			anchor_top=n;
			return;
		case Side::LEFT:
			anchor_left=n;
			return;
		case Side::RIGHT:
			anchor_right=n;
			return;
		case Side::BOTTOM:
			anchor_bottom=n;
			return;
		}
	}

void widget::set_anchors(float t,float b,float l,float r)
	{
	anchor_top=t;
	anchor_left=l;
	anchor_right=r;
	anchor_bottom=b;
	}

void widget::set_offset(Side side,int n)
	{
	switch (side)
		{
		case Side::TOP:
			offset_top=n;
			return;
		case Side::LEFT:
			offset_left=n;
			return;
		case Side::RIGHT:
			offset_right=n;
			return;
		case Side::BOTTOM:
			offset_bottom=n;
			return;
		}
	}

void widget::set_offsets(int t,int b,int l,int r)
	{
	offset_top=t;
	offset_left=l;
	offset_right=r;
	offset_bottom=b;
	}

void widget::set_anchors_preset(LayoutPreset preset)
	{
	switch (preset)
		{
		case LayoutPreset::TOP_LEFT:
			set_anchor(Side::TOP,0.0);
			set_anchor(Side::LEFT, 0.0);
			set_anchor(Side::BOTTOM,0.0);
			set_anchor(Side::RIGHT, 0.0);
			return;
		case LayoutPreset::TOP_RIGHT:
			set_anchor(Side::TOP,0.0);
			set_anchor(Side::LEFT, 1.0);
			set_anchor(Side::BOTTOM,0.0);
			set_anchor(Side::RIGHT, 1.0);
			return;
		case LayoutPreset::BOTTOM_LEFT:
			set_anchor(Side::TOP,1.0);
			set_anchor(Side::LEFT, 0.0);
			set_anchor(Side::BOTTOM,1.0);
			set_anchor(Side::RIGHT, 0.0);
			return;
		case LayoutPreset::BOTTOM_RIGHT:
			set_anchor(Side::TOP,1.0);
			set_anchor(Side::LEFT, 1.0);
			set_anchor(Side::BOTTOM,1.0);
			set_anchor(Side::RIGHT, 1.0);
			return;
		case LayoutPreset::CENTER:
			set_anchor(Side::TOP,0.5);
			set_anchor(Side::LEFT, 0.5);
			set_anchor(Side::BOTTOM,0.5);
			set_anchor(Side::RIGHT, 0.5);
			return;
		case LayoutPreset::CENTER_TOP:
			set_anchor(Side::TOP,0.0);
			set_anchor(Side::LEFT, 0.5);
			set_anchor(Side::BOTTOM,0.0);
			set_anchor(Side::RIGHT, 0.5);
			return;
		case LayoutPreset::CENTER_BOTTOM:
			set_anchor(Side::TOP,1.0);
			set_anchor(Side::LEFT, 0.5);
			set_anchor(Side::BOTTOM,1.0);
			set_anchor(Side::RIGHT, 0.5);
			return;
		case LayoutPreset::CENTER_LEFT:
			set_anchor(Side::TOP,0.5);
			set_anchor(Side::LEFT, 0.0);
			set_anchor(Side::BOTTOM,0.5);
			set_anchor(Side::RIGHT, 0.0);
			return;
		case LayoutPreset::CENTER_RIGHT:
			set_anchor(Side::TOP,0.5);
			set_anchor(Side::LEFT, 1.0);
			set_anchor(Side::BOTTOM,0.5);
			set_anchor(Side::RIGHT, 1.0);
			return;
		case LayoutPreset::WIDE_TOP:
			set_anchor(Side::TOP,0.0);
			set_anchor(Side::LEFT,0.0);
			set_anchor(Side::RIGHT,1.0);
			set_anchor(Side::BOTTOM,0.0);
		case LayoutPreset::WIDE_BOTTOM:
			set_anchor(Side::TOP,1.0);
			set_anchor(Side::LEFT,0.0);
			set_anchor(Side::RIGHT,1.0);
			set_anchor(Side::BOTTOM,1.0);
		case LayoutPreset::WIDE_LEFT:
			set_anchor(Side::TOP,0.0);
			set_anchor(Side::LEFT,0.0);
			set_anchor(Side::RIGHT,0.0);
			set_anchor(Side::BOTTOM,1.0);
		case LayoutPreset::WIDE_RIGHT:
			set_anchor(Side::TOP,0.0);
			set_anchor(Side::LEFT,1.0);
			set_anchor(Side::RIGHT,1.0);
			set_anchor(Side::BOTTOM,1.0);
		case LayoutPreset::FULL:
			set_anchor(Side::TOP,0.0);
			set_anchor(Side::LEFT, 0.0);
			set_anchor(Side::BOTTOM,1.0);
			set_anchor(Side::RIGHT, 1.0);
			return;
		}
	}

extentst viewscreenst::get_rect()
	{
	extentst rect;
	get_main_interface_dims(rect.sx,rect.ex);
	rect.sy=0;
	rect.ey=::init.display.grid_y;
	return rect;
	}

void viewscreenst::feed(std::set<InterfaceKey> &ev)
	{
	return widgets.feed(ev);
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

widget::~widget()
	{
	if (displaying_tooltip)
		{
		auto tooltip_widget=gview.grab_lastscreen()->widgets.add_or_get_widget<container>("Tooltip");
		tooltip_widget->set_active(false);
		displaying_tooltip=false;
		}
	}

void widget::render()
	{
	if (tooltip_type!=TooltipType::NONE && !tooltip.empty()&&enabler.tracking_on)
		{
		int32_t tmx=-1,tmy=-1;
		gps.get_mouse_text_coords(tmx,tmy);
		if (inside(tmy,tmx) && !displaying_tooltip)
			{
			displaying_tooltip=true;
			auto tooltip_widget=gview.grab_lastscreen()->widgets.add_or_get_widget<container>("Tooltip");
			tooltip_widget->set_parent(this);
			tooltip_widget->set_active(true);
			tooltip_widget->resize((int32_t)tooltip.length()/24+3,26);
			auto slice=tooltip_widget->add_or_get_widget<nineslice>("Background",&init.texpos_border_nw);
			slice->set_anchors_preset(LayoutPreset::FULL);
			auto text=tooltip_widget->add_or_get_widget<text_multiline>("Text");
			text->set_text(tooltip);
			text->fg=7;
			text->bright=1;
			text->min_w=24;
			text->set_offset(Side::LEFT,1);
			text->set_offset(Side::TOP,1);
			switch (tooltip_type)
				{
				case TooltipType::ABOVE:
					{
					tooltip_widget->set_anchors_preset(LayoutPreset::TOP_LEFT);
					tooltip_widget->set_offset(Side::TOP,-1-tooltip_widget->min_h);
					if (rect.sx>::init.display.grid_x/2) tooltip_widget->set_offset(Side::LEFT,-28);
					break;
					}
				case TooltipType::BELOW:
					{
					tooltip_widget->set_anchors_preset(LayoutPreset::BOTTOM_LEFT);
					tooltip_widget->set_offset(Side::TOP,1);
					if (rect.sx>::init.display.grid_x/2) tooltip_widget->set_offset(Side::LEFT,-28);
					break;
					}
				case TooltipType::REPLACE_MINIMAP:
					{
					//upper right
					//*************************** TEXTURE SIZE DEPENDENCE
					tooltip_widget->set_anchors_preset(LayoutPreset::TOP_RIGHT);
					tooltip_widget->set_parent(gview.grab_lastscreen());
					tooltip_widget->set_offsets(0,192/12-1,-1-192/8+1,-1);
					break;
					}
				}
			tooltip_widget->set_visible(true);
			tooltip_widget->arrange();
			}
		else if (displaying_tooltip && !inside(tmy,tmx))
			{
			auto tooltip_widget=gview.grab_lastscreen()->widgets.add_or_get_widget<container>("Tooltip");
			tooltip_widget->set_active(false);
			displaying_tooltip=false;
			}
		}
	}

void widget::feed(std::set<InterfaceKey> &ev)
	{
	if (custom_feed)
		{
		(*custom_feed)(ev);
		}
	}

int widget::width()
	{
	return max(rect.w(),min_w);
	}

int widget::height()
	{
	return max(rect.h(),min_h);
	}

void widget::resize(int h, int w) {
	min_w=w;
	min_h=h;
}

void widget::coords(int32_t& sy, int32_t& sx, int32_t& ey, int32_t& ex) {
    sx = rect.sx;
    sy = rect.sy;
    ex = rect.ex;
    ey = rect.ey;
}

bool widget::inside(int32_t y, int32_t x) {
    return x >= rect.sx && x <= rect.ex && y >= rect.sy && y <= rect.ey;
}

bool widget::set_active(bool n)
	{
	bool prev=!!(visibility_flags&WIDGET_VISIBILITY_ACTIVE);
	if (n) visibility_flags|=WIDGET_VISIBILITY_ACTIVE;
	else visibility_flags&=~WIDGET_VISIBILITY_ACTIVE;
	return prev;
	}

bool widget::set_visible(bool n)
	{
	bool prev=!!(visibility_flags&WIDGET_VISIBILITY_VISIBLE);
	if (n) visibility_flags|=WIDGET_VISIBILITY_VISIBLE;
	else visibility_flags&=~WIDGET_VISIBILITY_VISIBLE;
	return prev;
	}


folder::folder(std::shared_ptr<container> parent)
	{
	container_parent=parent;
	open=button([this](button *b)
		{
		if (enabler.mouse_lbut)
			{
			this->controlled_visible=!this->controlled_visible;
			enabler.mouse_lbut=false;
			}
		});
	open.set_parent(this);
	open.set_anchors_preset(LayoutPreset::WIDE_LEFT);
	open.set_offsets(0,0,1,0);
	label=text();
	label.fg=7;
	label.bg=0;
	label.bright=1;
	label.set_parent(this);
	label.set_anchors_preset(LayoutPreset::CENTER_LEFT);
	label.set_offset(Side::LEFT,4);
	last_visible=true;
	controlled_visible=true;
	}

void folder::arrange()
	{
	widget::arrange();
	open.arrange();
	label.arrange();
	if (last_visible==controlled_visible) return;
	auto ptr=container_parent.lock();
	for (auto &child:ptr->children)
		{
		if(controlled_set.contains(child)) child->set_active(controlled_visible);
		}
	last_visible=controlled_visible;
	parent->arrange();
	}

void folder::render()
	{
	widget::render();
	int32_t sy,sx,ey,ex;
	coords(sy,sx,ey,ex);
	draw_nineslice(init.texpos_neutral_intro_button,sy,sx,ey,ex,true);
	draw_nineslice(controlled_visible?init.texpos_down_arrow_button:init.texpos_up_arrow_button,sy,sx+1,ey,sx+3,false);
	label.render();
	}

void folder::feed(std::set<InterfaceKey> &ev) 
	{ 
	widget::feed(ev);
	open.feed(ev);
	arrange();
	}

filter::filter(std::shared_ptr<container> parent)
	{
	container_parent=parent;
	filter_idx=-1;
	all_filter.label=std::make_shared<text>("All");
	all_filter.label->set_parent(this);
	all_filter.label->fg=1;
	all_filter.label->bg=0;
	all_filter.label->bright=0;
	all_filter.click_button=std::make_shared<button>([this](button *b)
		{
		if (enabler.mouse_lbut)
			{
			this->set_filtered(-1);
			enabler.mouse_lbut=false;
			}
		});
	all_filter.click_button->set_parent(all_filter.label.get());
	all_filter.click_button->set_anchors_preset(LayoutPreset::FULL);
	}

int32_t filter::get_filtered()
	{
	return filter_idx;
	}

void filter::set_filtered(int32_t idx)
	{
	if (idx==filter_idx) return;
	filter_idx=idx;
	for (auto &f:filters)
		{
		f.label->fg=4;
		}
	auto ptr=container_parent.lock();
	if (filter_idx!=-1)
		{
		all_filter.label->fg=4;
		filters[filter_idx].label->fg=1;
		auto &controlled_set=filters[filter_idx].filtered_set;
		for (auto &child:ptr->children)
			{
			child->set_active(controlled_set.contains(child));
			}
		}
	else
		{
		all_filter.label->fg=1;
		for (auto &child:ptr->children)
			{
			child->set_active(true);
			}
		}
	ptr->arrange();
	}

// the risks we take
void multifilter::indiv_filter::set_filtered(int32_t idx)
	{
	if (idx==filter_idx) return;
	filter_idx=idx;
	for (auto &f:filters)
		{
		f.label->fg=4;
		}
	if (filter_idx!=-1)
		{
		all_filter.label->fg=4;
		filters[filter_idx].label->fg=1;
		}
	else
		{
		all_filter.label->fg=1;
		}
	}

void filter::add_new_filter(string name)
	{
	filters.emplace_back();
	auto &new_filter=filters.back();
	int32_t idx=(int32_t)filters.size()-1;
	new_filter.label=std::make_shared<text>(name);
	new_filter.label->set_parent(this);
	new_filter.label->set_offset(Side::TOP,idx+1);
	new_filter.label->fg=4;
	new_filter.label->bg=0;
	new_filter.label->bright=0;
	new_filter.click_button=std::make_shared<button>([this,idx](button *b)
		{
		if (enabler.mouse_lbut)
			{
			this->set_filtered(idx);
			enabler.mouse_lbut=false;
			}
		});
	new_filter.click_button->set_parent(new_filter.label.get());
	new_filter.click_button->set_anchors_preset(LayoutPreset::FULL);
	}

void filter::add_to_filter(string name,std::shared_ptr<widget> w)
	{
	auto filter_to_add_to=std::find_if(filters.begin(),filters.end(),[&name](auto &filter_entry)
		{
		return filter_entry.label->get_text()==name;
		});
	if (filter_to_add_to!=filters.end())
		{
		filter_to_add_to->filtered_set.insert(w);
		}
	}

void filter::remove_from_filter(string name,std::shared_ptr<widget> w)
	{
	auto filter_to_add_to=std::find_if(filters.begin(),filters.end(),[&name](auto &filter_entry)
		{
		return filter_entry.label->get_text()==name;
		});
	if (filter_to_add_to!=filters.end())
		{
		filter_to_add_to->filtered_set.erase(w);
		}
	}

void filter::clear_filters(std::shared_ptr<widget> w)
	{
	for (auto &filter:filters)
		{
		filter.filtered_set.erase(w);
		}
	}

bool filter::contains(std::shared_ptr<widget> w)
	{
	if (filter_idx==-1) return true;
	else return filters[filter_idx].filtered_set.contains(w);
	}

void filter::arrange()
	{
	widget::arrange();
	for (auto &f:filters)
		{
		f.label->arrange();
		f.click_button->arrange();
		}
	all_filter.label->arrange();
	all_filter.click_button->arrange();
	widget::arrange();
	}

void filter::render()
	{
	widget::render();
	for (auto &f:filters)
		{
		f.label->render();
		}
	all_filter.label->render();
	}


void filter::feed(std::set<InterfaceKey> &ev)
	{
	widget::feed(ev);
	all_filter.click_button->feed(ev);
	for (auto &f:filters)
		{
		f.click_button->feed(ev);
		}
	}

std::unordered_set<std::shared_ptr<widget>> &multifilter::indiv_filter::get_filtered_set()
	{
	return filters[filter_idx].filtered_set;
	}

multifilter::multifilter(std::shared_ptr<container> parent)
	{
	container_parent=parent;
	}

std::shared_ptr<multifilter::indiv_filter> multifilter::add_new_filter_block()
	{
	std::shared_ptr<indiv_filter> new_filter = std::make_shared<indiv_filter>(container_parent.lock());
	new_filter->set_parent(this);
	return filters.emplace_back(new_filter);
	}

void multifilter::arrange()
	{
	widget::arrange();
	int sy=0;
	for (auto &f:filters)
		{
		f->offset_top=sy;
		f->arrange();
		sy=f->get_rect().ey+2;
		}
	widget::arrange();
	}

void multifilter::render()
	{
	widget::render();
	for (auto &f:filters)
		{
		f->render();
		}
	}


void multifilter::feed(std::set<InterfaceKey> &ev)
	{
	widget::feed(ev);
	bool any_changed=false;
	for (auto &f:filters)
		{
		int32_t old=f->get_filtered();
		f->feed(ev);
		any_changed=any_changed||old!=f->get_filtered();
		}
	if (any_changed)
		{
		auto ptr=container_parent.lock();
		for (auto &child:ptr->children)
			{
			child->set_active(std::all_of(filters.begin(),filters.end(),[child](auto &f) { return f->contains(child); }));
			}
		ptr->arrange();
		}
	}

void container::feed(std::set<InterfaceKey>& events) {
    for (auto& child : children) {
        if (child->is_visible())
            child->feed(events);
    }
	widget::feed(events);
}

void container::logic() {
	widget::logic();
    for (auto& child : children) {
        if (child->is_visible())
            child->logic();
    }
}

void container::render() {
	widget::render();
    for (auto& child : children) {
        if (child->is_visible())
            child->render();
    }
}

void container::arrange()
{
	widget::arrange();
	for (auto &child:children)
		{
		child->arrange(); // want to do these even if they're invisible
		}
}

int container::visible_children()
	{
	return std::count_if(children.begin(),children.end(),[](auto &c) { return c->is_visible(); });
	}

int container::active_children()
	{
	return std::count_if(children.begin(),children.end(),[](auto &c) { return !!(c->visibility_flags&WIDGET_VISIBILITY_ACTIVE); });
	}

void container::remove_child(const string &id)
	{
	std::erase_if(children,[&](auto &w) { return w->name==id; });
	std::erase_if(children_by_name,[&](auto &pair) { return pair.first==id; });
	}

void container::remove_child(string &&id)
	{
	std::erase_if(children,[&](auto &w) { return w->name==id; });
	std::erase_if(children_by_name,[&](auto &pair) { return pair.first==id; });
	}

void container::remove_child(void *w)
	{
	std::erase_if(children,[&](auto &s_w) { return s_w.get()==w; });
	std::erase_if(children_by_name,[&](auto &pair) { return pair.second.get()==w; });
	}

void widget_stack::feed(std::set<InterfaceKey> &ev)
	{
	children.back()->feed(ev);
	widget::feed(ev);
	}

void rows_container::arrange() {
    int32_t y = 0;
    for (auto& child : children) {
		child->set_anchor(Side::TOP,0.0);
        if (child->is_visible()) {
            child->offset_top=y;
            y += child->height()+padding;
        }
    }
	container::arrange();
}

void columns_container::arrange() {
    int32_t x = 0;
    for (auto& child : children) {
		child->set_anchor(Side::LEFT,0.0);
        if (child->is_visible()) {
			child->offset_left=x;
            x += child->width()+padding;
        }
    }
	container::arrange();
}

void scroll_rows::logic()
{
	if (scrolling)
		{
		if (enabler.tracking_on&&enabler.mouse_lbut_down)
			{
			int32_t tmx=-1,tmy=-1;
			if (enabler.tracking_on)
				{
				gps.get_mouse_text_coords(tmx,tmy);
				}
			scrollbar.init(scroll,0,active_children()-1,num_visible,rect.sy,rect.ey);
			scrollbar.set_select_on_mouse_y(tmy);
			if(scroll!=scrollbar.sel)
				{
				scroll=scrollbar.sel;
				arrange();
				}
			}
		else scrolling=false;
		}
}

void scroll_rows::feed(set<InterfaceKey>& events) {
	if (active_children()>num_visible)
		{
		int32_t tmx=-1,tmy=-1;
		if (enabler.tracking_on)
			{
			gps.get_mouse_text_coords(tmx,tmy);
			}
		scrollbar.init(scroll,0,active_children()-1,num_visible,rect.sy,rect.ey);
		if (scrollbar.handle_events(events,scroll,scrolling))
			{
			arrange();
			}
		if (scrolling)
			{
			if (enabler.tracking_on&&enabler.mouse_lbut_down)
				{
				scrollbar.set_select_on_mouse_y(tmy);
				scroll=scrollbar.sel;
				}
			else
				{
				scrolling=false;
				}
			arrange();
			return;
			}
		else if (enabler.mouse_lbut&&tmy>=rect.sy&&tmy<=rect.ey&&tmx==rect.ex||tmx==rect.ex+1)
			{
			enabler.mouse_lbut=false;
			scrollbar.handle_initial_click(tmy,scroll,scrolling);
			arrange();
			}
		}
	rows_container::feed(events);
}

void scroll_rows::render() {
	container::render();
	if (active_children() <= num_visible) return;
    int32_t tmx = -1, tmy = -1;
    if (enabler.tracking_on)
    {
        gps.get_mouse_text_coords(tmx, tmy);
    }
    scrollbar.init(scroll, 0, active_children()-1,num_visible,rect.sy,rect.ey);
	//************************ DON'T ALWAYS USE BASIC SCROLL
		//scrollbar needs to understand if custom/mod graphics are available (ie, we are in dwarf/adv/legends mode and have loaded them)
    scrollbar.render(tmx, tmy, rect.ex, scrolling, SCROLLBAR_DISPLAY_FLAG_BASIC_SET);
}

void scroll_rows::scroll_to_top()
	{
	scroll=0;
	}

void scroll_rows::scroll_to_bottom()
	{
	scroll=active_children()-num_visible;
	}

bool widget_is_active(std::shared_ptr<widgets::widget> &w)
	{
	return !!(w->visibility_flags&WIDGET_VISIBILITY_ACTIVE);
	}

void scroll_rows::arrange() {
	move_to_anchor();
	for (auto &child:children)
		{
		child->set_parent(this); // allows for multiple parents for e.g. announcement interface
		child->set_visible(false);
		}
	if (scroll>=active_children()) scroll=0;
    int32_t sy = 0;
	num_visible=0;
	//for (auto &child:children|std::views::filter(&widget_is_active)|std::views::drop(scroll))
	for(int32_t i=scroll;i<children.size();++i)
		{
		auto &child=children[i];
		if(!widget_is_active(children[i]))continue;
		child->set_anchor(Side::TOP,0.0);
		child->set_anchor(Side::BOTTOM,0.0);
		child->move_to_anchor(); // need to do this to make sure it's the right size!
		const auto child_height=child->height();
		if ((child_height+sy)<=height())
			{
			child->set_visible(true);
			++num_visible;
			child->offset_top=sy;
			sy+=child_height+padding;
			}
		else
			{
			break;
			}
        }
	container::arrange();
}

table::table()
	{
	labels=add_widget<columns_container>();
	labels->set_anchors_preset(LayoutPreset::TOP_LEFT);
	labels->set_offsets(0,0,2,0);
	labels->padding=2;
	entries=add_widget<scroll_rows>();
	entries->set_anchors_preset(LayoutPreset::FULL);
	entries->set_offsets(2,-5,2,-1);
	entries->padding=1;
	}

void table::arrange()
	{
	if (!labels||!entries) return;
	move_to_anchor();
	for (auto i=0; i<labels->children.size(); ++i)
		{
		int this_w=labels->children[i]->min_w;
		for (auto &container_widget:entries->children)
			{
			if (!container_widget->is_container()) continue;
			auto entry=std::static_pointer_cast<container>(container_widget);
			this_w=max(this_w,entry->children[i]->width());
			}
		labels->children[i]->min_w=this_w;
		}
	labels->arrange();
	int max_x=0;
	for (auto &container_widget:entries->children)
		{
		max_x=max(container_widget->width(),max_x);
		if (!container_widget->is_container()) continue;
		auto entry=std::static_pointer_cast<container>(container_widget);
		for (auto i=0; i<labels->children.size(); ++i)
			{
			entry->children[i]->offset_left=labels->children[i]->offset_left;
			auto cur_x=entry->children[i]->width()+entry->children[i]->offset_left;
			max_x=max(cur_x,max_x);
			}
		}
	min_w=max_x;
	container::arrange();
	}

void tabs::add_tab_label(const char* s) {
	tab_labels.push_back(string(s));
}

void tabs::add_tab_label(const string& s) {
	tab_labels.push_back(s);
}

bool tabs::switch_tab(size_t which) {
    if (which == cur_idx || which<0) return false;
	cur_idx = which;
    arrange();
    return true;
}

void tabs::clear(){
	container::clear();
	tab_labels.clear();
	}

bool tabs::switch_tab(const string &s)
	{
	for (int32_t i=0; i<tab_labels.size(); ++i)
		{
		if (tab_labels[i]==s)
			{
			switch_tab(i);
			return true;
			}
		}
	return false;
	}

void tabs::feed(set<InterfaceKey>& events) {
	widget::feed(events);
	if(cur_idx<children.size())
		children[cur_idx]->feed(events);
    if (enabler.tracking_on && enabler.mouse_lbut) {
        int32_t tab_sx, tab_ex, tab_sy;
        tab_sx = rect.sx;
		tab_sy=rect.sy;
        int32_t mx = 0, my = 0;
        gps.get_mouse_text_coords(mx, my);
		for (auto &row:rows)
			{
			for (auto i=row.first; i<=row.second; ++i)
				{
				bool can_switch_to_tab=true;
				if (i<children.size()&&!(children[i]->visibility_flags&WIDGET_VISIBILITY_ACTIVE))
					{
					can_switch_to_tab=false;
					}
				int tab_width=(int32_t)tab_labels[i].length()+3;
				tab_ex=tab_sx+tab_width;
				if (can_switch_to_tab && my>=tab_sy&&my<=tab_sy+1&&mx>=tab_sx&&mx<=tab_ex)
					{
					bool succeeded=switch_tab(i);
					if (succeeded)
						{
						enabler.mouse_lbut=false;
						}
					return;
					}
				tab_sx+=tab_width+1;
				}
			tab_sy-=2;
			tab_sx=rect.sx;
			}
    }
}

void tabs::render() {
	widget::render();
    int32_t tab_sx, tab_ex,tab_sy;
    tab_sx = rect.sx;
	tab_sy=rect.sy;
	auto &tab_unselected=init.texpos_tab_unselected;
	auto &tab_selected=init.texpos_tab_selected;
	switch (tab_type)
		{
		case tabs::TabType::SHORT:
			tab_unselected=gps.texpos_short_tab;
			tab_selected=gps.texpos_short_tab_selected;
			break;
		case tabs::TabType::SHORT_SUB:
			tab_unselected=gps.texpos_short_subtab;
			tab_selected=gps.texpos_short_subtab_selected;
			break;
		case tabs::TabType::SHORT_SUBSUB:
			tab_unselected=gps.texpos_short_subsubtab;
			tab_selected=gps.texpos_short_subsubtab_selected;
			break;
		}
	for (auto &row:rows)
		{
		for (auto i=row.first; i<=row.second; ++i)
			{
			tab_ex=tab_sx+(int32_t)tab_labels[i].length()+3;
			auto &texpos=cur_idx==i?tab_selected:tab_unselected;
			// left border
			gps.locate(tab_sy,tab_sx);
			gps.add_lower_tile(texpos[0][0]);
			gps.locate(tab_sy+1,tab_sx);
			gps.add_lower_tile(texpos[0][1]);
			gps.locate(tab_sy,tab_sx+1);
			gps.add_lower_tile(texpos[1][0]);
			gps.locate(tab_sy+1,tab_sx+1);
			// right border
			gps.add_lower_tile(texpos[1][1]);
			gps.locate(tab_sy,tab_ex);
			gps.add_lower_tile(texpos[4][0]);
			gps.locate(tab_sy+1,tab_ex);
			gps.add_lower_tile(texpos[4][1]);
			gps.locate(tab_sy,tab_ex-1);
			gps.add_lower_tile(texpos[3][0]);
			gps.locate(tab_sy+1,tab_ex-1);
			gps.add_lower_tile(texpos[3][1]);
			// and the rest
			for (auto px=tab_sx+2; px<=tab_ex-2; px++)
				{
				gps.locate(tab_sy,px);
				gps.add_lower_tile(texpos[2][0]);
				gps.locate(tab_sy+1,px);
				gps.add_lower_tile(texpos[2][1]);
				}

			bool is_active=true;
			if (i<children.size())
				{
				is_active=!!(children[i]->visibility_flags&WIDGET_VISIBILITY_ACTIVE);
				}

				// labels
			if (!(::init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS)))
				{
				gps.locate(tab_sy+1,tab_sx+2);
				if (is_active)
					{
					if(cur_idx==i)gps.changecolor(7,0,1);
					else gps.changecolor(7,0,0);
					}
				else gps.changecolor(0,0,1);
				gps.addst(tab_labels[i]);
				}
			else
				{
				if (is_active)
					{
					if(cur_idx==i)
						{
						gps.color3ub(70,44,0);
						gps.bcolor3ub(0,0,0);
						}
					else
						{
						gps.changecolor(7,0,1);
						}
					}
				else
					{
					gps.color3ub(70,44,0);
					gps.bcolor3ub(0,0,0);
					}
				gps.locate(tab_sy,tab_sx+2);
				gps.addst_flag(tab_labels[i],justify_left,0,SCREENTEXPOS_FLAG_TOP_OF_TEXT);
				gps.locate(tab_sy+1,tab_sx+2);
				gps.addst_flag(tab_labels[i],justify_left,0,SCREENTEXPOS_FLAG_BOTTOM_OF_TEXT);
				}
			tab_sx+=(int32_t)tab_labels[i].length()+4;
			}
		tab_sy-=2;
		tab_sx=rect.sx;
		}
	if (cur_idx < children.size())
		children[cur_idx]->render();
}

void tabs::arrange() {
	widget::arrange();
	int cur_start=0;
	rows.clear();
	int cur_sx=rect.sx;
	for (int32_t i=0; i<tab_labels.size(); ++i)
		{
		int32_t length=(int32_t)tab_labels[i].length()+3;
		if(cur_sx+length>=rect.ex)
			{
			//rows.push_back({cur_start,max(i,1)-1});
			rows.push_back(std::make_pair(cur_start,max(i,1)-1));
			cur_start=i;
			cur_sx=rect.sx;
			}
		cur_sx+=length+1;
		}
	//rows.push_back({ cur_start,tab_labels.size()-1});
	rows.push_back(std::make_pair(cur_start,tab_labels.size()-1));
	offset_top=1+((int32_t)rows.size()-1)*2;
	widget::arrange();
	size_t which_is_current=0;
	for (auto i=0;i<rows.size();++i)
		{
		if (cur_idx>=rows[i].first&&cur_idx<=rows[i].second) which_is_current=i;
		}
	std::iter_swap(rows.begin(),rows.begin()+which_is_current);
	for (auto& tab : children) {
		tab->set_visible(false);
	}
	if (cur_idx>=children.size()) return;
	auto& cur_tab = children[cur_idx];
	cur_tab->set_anchors_preset(LayoutPreset::FULL);
	cur_tab->set_offsets(3,-2,1,0);
	cur_tab->set_visible(true);
	container::arrange();
	cur_tab->arrange();// arranging this one last gives it "priority", to solve various issues--arranging something twice in one frame should never cause problems
}

button::button(std::function<void(button*)> c) : widget() {
    callback = c;
}

void button::feed(set<InterfaceKey>& events) {
    if (enabler.tracking_on) {
        int32_t mx = 0, my = 0;
        gps.get_mouse_text_coords(mx, my);
        if (inside(my, mx)) {
            callback(this);
        }
    }
	widget::feed(events);
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
			return;
		}
	}
	widget::feed(events);
}
void better_button::render() {
	widget::render();
	int32_t sy, sx, ey, ex;
	coords(sy, sx, ey, ex);
	int32_t* tex = texpos();
	if (tex) draw_nineslice(tex, sy, sx, ey, ex);
	if (display_string)
		{
		string s = display_string();
		if (s.size() > 0) 
			{
			gps.locate((sy + ey) / 2, (sx+ex)/2 - ((int32_t)s.size()/2));
			gps.changecolor(7, 0, 1);
			gps.addst(s, justify_center);
			}
		}
}

void interface_main_button::render()
	{
	widget::render();
	int32_t sy,sx,ey,ex;
	coords(sy,sx,ey,ex);
	for (int py=sy; py<sy+3; ++py)
		{
		for (int px=sx; px<sx+4; ++px)
			{
			gps.add_tile(gps.texpos_button_main[which_button][px][py],0);
			}
		}
	}

textbox::textbox(int len, uint32_t flag) : widget() {
    str = "";
    maxlen = len;
    flags = flag;
    fg = 7;
    bg = 0;
    bright = 1;
    min_w = len + 6;
    min_h = 3;
	input = false;
    toggle = button([this](button* b) {
        if (enabler.mouse_lbut) {
            this->input = !this->input;
            enabler.mouse_lbut = false;
        }
        });
    set_callback([](auto _) {});
}

void textbox::feed(set<InterfaceKey> &events) {
	widget::feed(events);
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
	widget::render();
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

void textbox::arrange()
	{
	toggle.set_anchors_preset(LayoutPreset::WIDE_RIGHT);
	toggle.offset_left=-4;
	widget::arrange();
	}

text::text(const string& s) {
    fg = 7;
    bg = 0;
    bright = 1;
    set_text(s);
}

text::text(string &&s)
	{
    fg = 7;
    bg = 0;
    bright = 1;
    set_text(s);
	}

void text::render() {
	widget::render();
    gps.locate(rect.sy, rect.sx);
    gps.changecolor(fg, bg, bright);
    gps.addst(str);
}

void text_multiline::arrange()
	{
	if (min_w>str.length()) min_h=((int32_t)str.length()/min_w)+1;
	widget::arrange();
	}

void text_multiline::render()
	{
	widget::render();
	separate_string(str,strs,min_w);
	for (int i=0; i<strs.size(); ++i)
		{
		gps.locate(rect.sy+i,rect.sx);
		gps.changecolor(fg,bg,bright);
		gps.addst(strs[i]);
		}
	}

void nineslice::render() {
	widget::render();
    int32_t sy, sx, ey, ex;
    coords(sy, sx, ey, ex);
    draw_nineslice(selected_texpos, sy, sx, ey, ex, true);
}

dropdown::dropdown() : widget() {
    cur_selected = 0;
	min_h=3;
    set_callback([](auto& s) {}); // no-op, not sure if null std::functions crash
}

dropdown::dropdown(std::initializer_list<string> new_options) : widget() {
    cur_selected = 0;
    min_h = 3;
    std::copy(new_options.begin(), new_options.end(), options.begin());
    for (auto& s : options) {
        min_w = max(min_w, (int32_t)s.length() + 4);
    }
    set_callback([](auto& s) {});
}

void dropdown::add_option(std::string& s) {
    options.push_back(s);
    min_w = max(min_w, (int32_t)s.length() + 4);
}

void dropdown::feed(set<InterfaceKey>& events) {
	// TODO TODO DON'T FORGET THIS WOW
	// whoops
}

void dropdown::render() {
	widget::render();
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