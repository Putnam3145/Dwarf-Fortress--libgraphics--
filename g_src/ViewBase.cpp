#include <algorithm>
#include <assert.h>
#include <iostream>
#include "ViewBase.h"
#include "interface.h"
#include "init.h"


using namespace widgets;

bool standardscroll(std::set<InterfaceKey> &events,int32_t &scroll,const int32_t page_size) {
	if (events.contains(INTERFACEKEY_STANDARDSCROLL_UP))
		{
		--scroll;
		events.erase(INTERFACEKEY_STANDARDSCROLL_UP);
		return true;
		}
	else if (events.contains(INTERFACEKEY_STANDARDSCROLL_DOWN))
		{
		++scroll;
		events.erase(INTERFACEKEY_STANDARDSCROLL_DOWN);
		return true;
		}
	else if (events.contains(INTERFACEKEY_STANDARDSCROLL_PAGEUP))
		{
		scroll-=page_size;
		events.erase(INTERFACEKEY_STANDARDSCROLL_PAGEUP);
		return true;
		}
	else if (events.contains(INTERFACEKEY_STANDARDSCROLL_PAGEDOWN))
		{
		scroll+=page_size;
		events.erase(INTERFACEKEY_STANDARDSCROLL_PAGEDOWN);
		return true;
		}
	return false;
	}

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

void blank_anchors(int32_t sy,int32_t sx,int32_t ey,int32_t ex) {
	for (auto px=sx; px<=ex; ++px)
		{
		for (auto py=sy; py<=ey; ++py)
			{
			gps.screentexpos_anchored[sx*gps.dimy + sy]=0;
			}
		}
	}

widget::widget() {
	flag=WIDGET_VISIBILITY_ACTUALLY_VISIBLE|WIDGET_CAN_KEY_ACTIVATE;
	name = "";
	tooltip="";
	tooltip_type=TooltipType::NONE;
	set_anchors_preset(LayoutPreset::TOP_LEFT);
	set_offsets(0,0,0,0);
	min_h=min_w=0;
}

void widget::move_to_anchor() {
	// for the opinionated: the parens are for clarity, it changes no semantics, i know this
	extentst parent_rect;
	if (!(flag&WIDGET_GLOBAL_POSITIONING))
		{
		if (parent)
			{
			parent_rect=parent->get_rect();
			}
		else
			{
			parent_rect=gview.grab_lastscreen()->get_rect();
			}
		}
	else
		{
		parent_rect.sx=0;
		parent_rect.sy=0;
		parent_rect.ex=::init.display.grid_x;
		parent_rect.ey=::init.display.grid_y;
		}
    rect.sx=parent_rect.sx+((parent_rect.ex-parent_rect.sx)*anchor_left)+offset_left;
	rect.ex=parent_rect.sx+((parent_rect.ex-parent_rect.sx)*anchor_right)+offset_right;
    rect.sy=parent_rect.sy+((parent_rect.ey-parent_rect.sy)*anchor_top)+offset_top;
	rect.ey=parent_rect.sy+((parent_rect.ey-parent_rect.sy)*anchor_bottom)+offset_bottom;
	if (rect.w()<min_w)
		{
		if (anchor_left>0.5 && anchor_right>0.5)
			{
			rect.sx=rect.ex-min_w+1;
			}
		else
			{
			rect.ex=rect.sx+min_w-1;
			}
		}
	if (rect.h()<min_h)
		{
		if (anchor_top>0.5 && anchor_bottom>0.5)
			{
			rect.sy=rect.ey-min_h+1;
			}
		else
			{
			rect.ey=rect.sy+min_h-1;
			}
		}
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
			return;
		case LayoutPreset::WIDE_BOTTOM:
			set_anchor(Side::TOP,1.0);
			set_anchor(Side::LEFT,0.0);
			set_anchor(Side::RIGHT,1.0);
			set_anchor(Side::BOTTOM,1.0);
			return;
		case LayoutPreset::WIDE_LEFT:
			set_anchor(Side::TOP,0.0);
			set_anchor(Side::LEFT,0.0);
			set_anchor(Side::RIGHT,0.0);
			set_anchor(Side::BOTTOM,1.0);
			return;
		case LayoutPreset::WIDE_RIGHT:
			set_anchor(Side::TOP,0.0);
			set_anchor(Side::LEFT,1.0);
			set_anchor(Side::RIGHT,1.0);
			set_anchor(Side::BOTTOM,1.0);
			return;
		case LayoutPreset::FULL:
			set_anchor(Side::TOP,0.0);
			set_anchor(Side::LEFT, 0.0);
			set_anchor(Side::BOTTOM,1.0);
			set_anchor(Side::RIGHT, 1.0);
			return;
		case LayoutPreset::HALF_LEFT:
			set_anchor(Side::TOP,0.0);
			set_anchor(Side::LEFT,0.0);
			set_anchor(Side::BOTTOM,1.0);
			set_anchor(Side::RIGHT,0.5);
			return;
		case LayoutPreset::HALF_RIGHT:
			set_anchor(Side::TOP,0.0);
			set_anchor(Side::LEFT,0.5);
			set_anchor(Side::BOTTOM,1.0);
			set_anchor(Side::RIGHT,1.0);
			return;
		case LayoutPreset::HALF_TOP:
			set_anchor(Side::TOP,0.0);
			set_anchor(Side::LEFT,0.0);
			set_anchor(Side::BOTTOM,0.5);
			set_anchor(Side::RIGHT,1.0);
			return;
		case LayoutPreset::HALF_BOTTOM:
			set_anchor(Side::TOP,0.5);
			set_anchor(Side::LEFT,0.0);
			set_anchor(Side::BOTTOM,1.0);
			set_anchor(Side::RIGHT,1.0);
			return;
		}
	}

void widget::set_layout_preset(LayoutPreset preset) {
	set_anchors_preset(preset);
	switch (preset)
		{
		case LayoutPreset::TOP_LEFT:
		case LayoutPreset::TOP_RIGHT:
		case LayoutPreset::BOTTOM_LEFT:
		case LayoutPreset::BOTTOM_RIGHT:
			set_offsets(0,0,0,0);
			return;
		case LayoutPreset::CENTER:
			{
			move_to_anchor();
			const auto w=width()/2;
			const auto h=height()/2;
			set_offsets(-h,h,-w,w);
			return;
			}
		case LayoutPreset::CENTER_TOP:
		case LayoutPreset::CENTER_BOTTOM:
			{
			const auto w=width()/2;
			move_to_anchor();
			set_offsets(0,0,-w,w);
			return;
			}
		case LayoutPreset::CENTER_LEFT:
		case LayoutPreset::CENTER_RIGHT:
			{
			const auto h=height()/2;
			move_to_anchor();
			set_offsets(-h,h,0,0);
			return;
			}
		case LayoutPreset::WIDE_TOP:
		case LayoutPreset::WIDE_BOTTOM:
			set_offsets(0,0,0,0);
			min_w=0;
			return;
		case LayoutPreset::WIDE_LEFT:
		case LayoutPreset::WIDE_RIGHT:
			set_offsets(0,0,0,0);
			min_h=0;
			return;
		case LayoutPreset::FULL:
			set_offsets(0,0,0,0);
			min_h=min_w=0;
			return;
		case LayoutPreset::HALF_LEFT:
			set_offsets(0,0,0,0);
			return;
		case LayoutPreset::HALF_RIGHT:
			set_offsets(0,0,1,0);
			return;
		case LayoutPreset::HALF_TOP:
			set_offsets(0,0,0,0);
			return;
		case LayoutPreset::HALF_BOTTOM:
			set_offsets(0,1,0,0);
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

void draw_nineslice(int32_t *texpos,int sy,int sx,int ey,int ex,override_tile_type override) {
	// corners
	gps.locate(sy,sx);
	gps.add_lower_tile(texpos[0]);
	gps.locate(sy,ex);
	gps.add_lower_tile(texpos[2]);
	gps.locate(ey,sx);
	gps.add_lower_tile(texpos[6]);
	gps.locate(ey,ex);
	gps.add_lower_tile(texpos[8]);
	// top and bottom
	for (auto bx=sx + 1; bx <= ex - 1; bx++)
		{
		gps.locate(sy,bx);
		gps.add_lower_tile(texpos[1]);
		gps.locate(ey,bx);
		gps.add_lower_tile(texpos[7]);
		}
	// sides and middle
	for (auto by=sy + 1; by <= ey - 1; by++)
		{
		gps.locate(by,sx);
		gps.add_lower_tile(texpos[3]);
		gps.locate(by,ex);
		gps.add_lower_tile(texpos[5]);
		for (auto bx=sx + 1; bx <= ex - 1; bx++)
			{
			gps.locate(by,bx);
			gps.add_lower_tile(texpos[4]);
			}
		}
	if (::init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))
		{
		gps.override_tiles(sy,sx,ey,ex,override);
		}
	}

void draw_horizontal_nineslice(int32_t *texpos,int sy,int sx,int ey,int ex,override_tile_type override) {
	// corners
	gps.locate(sy,sx);
	gps.add_lower_tile(texpos[0]);
	gps.locate(sy,ex);
	gps.add_lower_tile(texpos[6]);
	gps.locate(ey,sx);
	gps.add_lower_tile(texpos[2]);
	gps.locate(ey,ex);
	gps.add_lower_tile(texpos[8]);
	// top and bottom
	for (auto bx=sx + 1; bx <= ex - 1; bx++)
		{
		gps.locate(sy,bx);
		gps.add_lower_tile(texpos[3]);
		gps.locate(ey,bx);
		gps.add_lower_tile(texpos[5]);
		}
	// sides and middle
	for (auto by=sy + 1; by <= ey - 1; by++)
		{
		gps.locate(by,sx);
		gps.add_lower_tile(texpos[1]);
		gps.locate(by,ex);
		gps.add_lower_tile(texpos[7]);
		for (auto bx=sx + 1; bx <= ex - 1; bx++)
			{
			gps.locate(by,bx);
			gps.add_lower_tile(texpos[4]);
			}
		}
	if (::init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))
		{
		gps.override_tiles(sy,sx,ey,ex,override);
		}
	}

void draw_sort_widget(int sy,int sx,int ey,int ex,bool active,bool ascending)
	{
	int32_t *texpos=NULL;
	if (ascending)
		{
		texpos=(active)?(::init.texpos_sort_ascending_active):(::init.texpos_sort_ascending_inactive);
		}
	else
		{
		texpos=(active)?(::init.texpos_sort_descending_active):(::init.texpos_sort_descending_inactive);
		}
	if (texpos[0]==0)
		{
		gps.locate(sy,sx);
		if (active)
			{
			gps.changecolor(7,0,1);
			}
		else
			{
			gps.changecolor(0,0,1);
			}
		if (ascending)
			{
			gps.addst(" ^ ");
			}
		else
			{
			gps.addst(" v ");
			}
		}
	else
		{
		for (auto px=0; px<=3; ++px)
			{
			gps.locate(sy,sx+px);
			gps.add_lower_tile(texpos[px]);
			}
		}
	}

void draw_sort_widget(int sy,int sx,int ey,int ex,bool active,bool ascending, const string &name)
	{
	draw_sort_widget(sy,sx,ey,ex,active,ascending);
	const auto print_thing=[sy](int32_t sx,int32_t ex,int32_t *texpos) {
		gps.locate(sy,sx);
		gps.add_lower_tile(texpos[0]);
		for (auto px=sx+1; px<ex; ++px)
			{
			gps.locate(sy,px);
			gps.add_lower_tile(texpos[1]);
			}
		gps.locate(sy,ex);
		gps.add_lower_tile(texpos[2]);
		};
	if (::init.texpos_sort_text_active[0]==0)
		{
		if (active)
			{
			gps.changecolor(7,0,1);
			}
		else
			{
			gps.changecolor(0,0,1);
			}
		gps.addst(name);
		}
	else
		{
		if (active)
			{
			print_thing(sx+4,ex,::init.texpos_sort_text_active);
			gps.locate(sy,sx+5);
			if (init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))	gps.changecolor(0,0,0);
			else gps.changecolor(7,0,1);
			gps.addst(name);
			}
		else
			{
			print_thing(sx+4,ex,::init.texpos_sort_text_inactive);
			gps.locate(sy,sx+5);
			if (init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS)) gps.changecolor(0,0,1);
			else gps.changecolor(0,0,1);
			gps.addst(name);
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

bool widget::can_key_activate() {
	return !!(flag & WIDGET_CAN_KEY_ACTIVATE);
	}

bool widget::set_can_key_activate(bool can, bool children_too) {
	const auto old=can_key_activate();
	if (can)
		{
		flag|=WIDGET_CAN_KEY_ACTIVATE;
		}
	else
		{
		flag&=~WIDGET_CAN_KEY_ACTIVATE;
		}
	if (children_too)
		{
		auto c=dynamic_cast<container *>(this);
		if (c)
			{
			for (auto &child : c->children)
				{
				child->set_can_key_activate(can);
				}
			}
		}
	return old;
	}

// does nothing on its own! inheritors might want to call it for various reasons, though
bool widget::activate() {
	bool ret=false;
	for (auto &f : custom_activated) 
		{ 
		if (f(this)) ret=true;
		}
	return ret;
	}

void widget::locate(int32_t offset_y, int32_t offset_x) {
	gps.locate(rect.sy+offset_y,rect.sx+offset_x);
	}

void widget::render(uint32_t curtick)
	{
	for (auto &fn : custom_render)
		{
		fn(this, curtick);
		}
	if (enabler.tracking_on)
		{
		int32_t tmx=-1,tmy=-1;
		gps.get_mouse_text_coords(tmx,tmy);
		if (inside(tmy,tmx))
			{
			if (tooltip_type!=TooltipType::NONE && std::holds_alternative<string>(tooltip))
				{
				auto &tooltip_str=std::get<string>(tooltip);
				displaying_tooltip=true;
				auto tooltip_widget=gview.grab_lastscreen()->widgets.add_or_get_widget<container>("Tooltip");
				tooltip_widget->set_parent(this);
				tooltip_widget->set_active(true);
				tooltip_widget->resize((int32_t)tooltip_str.length()/24+3,26);
				tooltip_widget->tooltip_type=tooltip_type;
				auto slice=tooltip_widget->add_or_get_widget<nineslice>("Background",&init.texpos_border_nw);
				slice->set_anchors_preset(LayoutPreset::FULL);
				auto text=tooltip_widget->add_or_get_widget<text_multiline>("Text");
				text->set_text(tooltip_str);
				text->set_color(7,0,1);
				text->set_layout_preset(LayoutPreset::WIDE_TOP);
				text->set_offset(Side::LEFT,1);
				text->set_offset(Side::TOP,1);
				auto hotkeys=tooltip_widget->add_or_get_widget<columns_container>("Hotkeys");
				hotkeys->clear();
				hotkeys->set_anchors_preset(widgets::LayoutPreset::BOTTOM_LEFT);
				hotkeys->offset_bottom=-2;
				hotkeys->offset_left=1;
				for (auto &key : activation_hotkeys)
					{
					auto c=hotkeys->add_widget<widgets::columns_container>();
					c->add_widget<widgets::text>("Hotkey: ");
					c->add_widget<widgets::keybinding_display>(key);
					}
				tooltip_widget->set_global_positioning(false); // want this by default...
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
						tooltip_widget->set_offsets(0,192/12+1,-192/8+1,0);
						tooltip_widget->set_global_positioning(true); // but want it to be global for this, since minimap is
						break;
						}
					}
				tooltip_widget->set_visible(true);
				tooltip_widget->arrange();
				}
			else if(std::holds_alternative<std::function<void()>>(tooltip))
				{
				std::get<std::function<void()>>(tooltip)();
				}
			}
		else if (displaying_tooltip && !inside(tmy,tmx))
			{
			auto tooltip_widget=gview.grab_lastscreen()->widgets.add_or_get_widget<container>("Tooltip");
			tooltip_widget->set_active(false);
			displaying_tooltip=false;
			tooltip_widget->tooltip_type=TooltipType::NONE;
			}
		}
	}

void widget::feed(std::set<InterfaceKey> &ev)
	{
	// click-to-activate is such a common thing it's part of the "default behavior" here--custom feeds happen **first** so
	// that custom feed functions can completely intercept the activation, which is far more specialized
	// (but still highly useful for e.g. certain containers that might want to be keyboard controlled)
	for (auto &fn : custom_feed)
		{
		fn(ev, this);
		}
	if (enabler.tracking_on && enabler.mouse_lbut)
		{
		int32_t mx=0,my=0;
		gps.get_mouse_text_coords(mx,my);
		if (inside(my,mx))
			{
			if (activate())
				{
				enabler.mouse_lbut=false;
				}
			}
		}
	if (can_key_activate())
		{
		static std::vector<InterfaceKey> inter;
		inter.clear();
		std::set_intersection(activation_hotkeys.begin(),activation_hotkeys.end(),ev.begin(),ev.end(),std::back_inserter(inter));
		if (!inter.empty())
			{
			if (activate())
				{
				for (auto &i : inter)
					{
					ev.erase(i);
					}
				}
			}
		}
	}

void widget::logic() 
	{
	for (auto &fn : custom_logic)
		{
		fn(this);
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
    return is_visible() && x >= rect.sx && x <= rect.ex && y >= rect.sy && y <= rect.ey;
}

template<typename T>
bool set_flag_ret_old(T &flag,T which_flag, bool set) {
	bool prev=!!(flag&which_flag);
	if (set) flag|=which_flag;
	else flag&=~which_flag;
	return prev;
	}

bool widget::set_active(bool n)
	{
	return set_flag_ret_old(flag,WIDGET_VISIBILITY_ACTIVE,n);
	}

bool widget::set_visible(bool n)
	{
	auto prev=set_flag_ret_old(flag,WIDGET_VISIBILITY_VISIBLE,n);
	// Not done for set_active on purpose!
	if (auto c=dynamic_cast<container *>(this); c)
		{
		for (auto &child : c->children)
			{
			child->set_visible(n);
			}
		}
	return prev;
	}

bool widget::set_global_positioning(bool n) {
	return set_flag_ret_old(flag,WIDGET_GLOBAL_POSITIONING,n);
	}

void widget::clear() {}


folder::folder(std::shared_ptr<container> parent)
	{
	container_parent=parent;
	open=widget(); 
	open.set_custom_activated([this](widget *w)
		{
		this->controlled_visible=!this->controlled_visible;
		return true;
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

void folder::render(uint32_t curtick)
	{
	widget::render(curtick);
	int32_t sy,sx,ey,ex;
	coords(sy,sx,ey,ex);
	draw_nineslice(init.texpos_neutral_intro_button,sy,sx,ey,ex,true);
	draw_nineslice(controlled_visible?init.texpos_down_arrow_button:init.texpos_up_arrow_button,sy,sx+1,ey,sx+3,false);
	label.render(curtick);
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
	all_filter.label->set_custom_activated([this](widget *b)
		{
		this->set_filtered(-1);
		return true;
		});
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
	new_filter.label->set_custom_activated([this,idx](widget *b)
		{
			this->set_filtered(idx);
			return true;
		});
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
		}
	all_filter.label->arrange();
	widget::arrange();
	}

void filter::render(uint32_t curtick)
	{
	widget::render(curtick);
	for (auto &f:filters)
		{
		f.label->render(curtick);
		}
	all_filter.label->render(curtick);
	}


void filter::feed(std::set<InterfaceKey> &ev)
	{
	widget::feed(ev);
	all_filter.label->feed(ev);
	for (auto &f:filters)
		{
		f.label->feed(ev);
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

void multifilter::render(uint32_t curtick)
	{
	widget::render(curtick);
	for (auto &f:filters)
		{
		f->render(curtick);
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

void container::render(uint32_t curtick) {
	widget::render(curtick);
    for (auto& child : children) {
        if (child->is_visible())
            child->render(curtick);
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

std::shared_ptr<widget> container::get_by_name(const std::string &s) {
	for (auto &child : children)
		{
		if (child->name==s)
			{
			return child;
			}
		else if(auto c=std::dynamic_pointer_cast<container>(child); c)
			{
			auto maybe_child=c->get_by_name(s);
			if (maybe_child)
				{
				return maybe_child;
				}
			}
		}
	return nullptr;
	}

int container::visible_children()
	{
	return std::count_if(children.begin(),children.end(),[](auto &c) { return c->is_visible(); });
	}

int container::active_children()
	{
	return std::count_if(children.begin(),children.end(),[](auto &c) { return !!(c->flag&WIDGET_VISIBILITY_ACTIVE); });
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

void widget_stack::deferred_replace(std::shared_ptr<widget> w) {
	if (children.empty())
		{
		add_widget<widget>(w);
		}
	else
		{
		deferred_replacement=w;
		}
	}

void widget_stack::do_replacements() {
	if (deferred_replacement)
		{
		clear();
		do_pop=false;
		add_widget<widget>(deferred_replacement);
		deferred_replacement.reset();
		}
	if (do_pop)
		{
		auto child=children.back();
		children.pop_back();
		if (!child->name.empty())
			{
			remove_child(child->name);
			}
		do_pop=false;
		}
	}

void widget_stack::render(uint32_t curtick) {
	if (children.empty()) return;
	do_replacements();
	children.back()->render();
	widget::render();
	}

void widget_stack::arrange() {
	if (children.empty()) return;
	for (auto &child : children)
		{
		child->set_visible(false);
		}
	children.back()->set_visible(true);
	container::arrange();
	}

void widget_stack::feed(std::set<InterfaceKey> &ev)
	{
	if (children.empty()) return;
	do_replacements();
	children.back()->feed(ev);
	widget::feed(ev);
	}

void rows_container::arrange() {
    int32_t y = 0;
    for (auto& child : children) {
		child->set_anchor(Side::TOP,0.0);
		child->arrange();
		if (child->is_visible()) {
            child->offset_top=y;
            y += child->height()+padding;
        }
    }
	min_h=y-padding; // if you want a custom min_h, you are using the wrong container
	container::arrange();
}

void columns_container::arrange() {
    int32_t x = 0;
    for (auto& child : children) {
		child->set_anchor(Side::LEFT,0.0);
		child->arrange();
        if (child->is_visible()) {
			child->offset_left=x;
            x += child->width()+padding;
        }
    }
	min_w=x-padding; // if you want a custom min_w, you are using the wrong container
	container::arrange();
}

void scroll_rows::handle_scroll(set<InterfaceKey> &events) {
	if (active_children()>num_visible)
		{
		int32_t tmx=-1,tmy=-1;
		if (enabler.tracking_on)
			{
			gps.get_mouse_text_coords(tmx,tmy);
			}
		scrollbar.init(scroll,0,active_children()-1,num_visible,rect.sy+1,rect.ey-1);
		if (inside(tmy,tmx) && scrollbar.handle_events(events,scroll,scrolling))
			{
			arrange();
			}
		if (!enabler.mouse_lbut_down)
			{
			scrolling=false;
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
		else if (enabler.mouse_lbut&&tmy>=rect.sy&&tmy<=rect.ey&&tmx==rect.ex+1||tmx==rect.ex+2)
			{
			enabler.mouse_lbut=false;
			scrollbar.handle_initial_click(tmy,scroll,scrolling);
			arrange();
			}
		}
	}

void scroll_rows::feed(set<InterfaceKey>& events) {
	handle_scroll(events);
	rows_container::feed(events);
}

void scroll_rows::render(uint32_t curtick) {
	container::render(curtick);
	if (active_children() <= num_visible) return;
    int32_t tmx = -1, tmy = -1;
    if (enabler.tracking_on)
    {
        gps.get_mouse_text_coords(tmx, tmy);
    }
	if (enabler.mouse_lbut_down)
		{
		auto dummy=std::set<InterfaceKey>();
		handle_scroll(dummy);
		}
	else
		{
		scrolling=false;
		}
    scrollbar.init(scroll, 0, active_children()-1,num_visible,rect.sy+1,rect.ey-1);
	//************************ DON'T ALWAYS USE BASIC SCROLL
		//scrollbar needs to understand if custom/mod graphics are available (ie, we are in dwarf/adv/legends mode and have loaded them)
    scrollbar.render(tmx, tmy, rect.ex+1, scrolling,scrollbar_display_flags);
}

void scroll_rows::scroll_to_top()
	{
	scroll=0;
	}

void scroll_rows::scroll_to_bottom()
	{
	scroll=active_children()-num_visible;
	}

void scroll_rows::page_down() {
	scroll=min(scroll+num_visible, active_children()-num_visible);
	}

void scroll_rows::page_up() {
	scroll=max(scroll-num_visible,0);
	}

int32_t scroll_rows::page_size() {
	return num_visible;
	}

void scroll_rows::scroll_to(int32_t idx) {
	if (num_visible==0) scroll=0;
	else if (idx < scroll) scroll=idx;
	else if (idx >= scroll+num_visible) scroll=idx-num_visible+1;
	}

bool widget_is_active(std::shared_ptr<widgets::widget> &w)
	{
	return !!(w->flag&WIDGET_VISIBILITY_ACTIVE);
	}

void scroll_rows::arrange() {
	move_to_anchor();
	for (auto &child:children)
		{
		child->set_parent(this); // allows for multiple parents for e.g. announcement interface
		child->set_visible(true);
		}
	if (scroll>=active_children()) scroll=0;
    int32_t sy = 0;
	num_visible=0;
	int32_t s=scroll;
	for (auto &child:children)
		{
		if (!widget_is_active(child))continue;
		--s;
		if (s>=0)
			{
			child->set_visible(false);
			continue;
			};
		child->set_anchor(Side::TOP,0.0);
		child->set_anchor(Side::BOTTOM,0.0);
		if (child->anchor_right==1.0) child->offset_right=-1; // this ensures it won't overlap the scroll bar
		child->arrange(); // need to do this to make sure it's the right size!
		const auto child_height=child->height();
		if ((child_height+sy)<height())
			{
			++num_visible;
			child->offset_top=sy;
			sy+=child_height+padding;
			}
		else
			{
			child->set_visible(false);
			}
        }
	scroll_to(scroll); // Makes sure if we're out of range on scroll, we get back in range
	container::arrange();
}

radio_rows::radio_rows() {
	rows.set_parent(this);
	selected=NULL;
	rows.set_anchors_preset(widgets::LayoutPreset::FULL);
	}

std::shared_ptr<container> radio_rows::add_entry(const string &s,std::function<void(widget *)> f) {
	auto row_idx=rows.children.size();
	auto new_row=rows.add_widget<container>();
	new_row->min_h=3;
	new_row->set_anchors_preset(widgets::LayoutPreset::WIDE_TOP);
	new_row->offset_right=-1;
	auto t=new_row->add_or_get_widget<text>("Text",s);
	t->set_anchors_preset(widgets::LayoutPreset::CENTER_LEFT);
	t->offset_left=2;
	auto n=new_row->add_or_get_widget<nineslice_horizontal>("Slice",(int32_t*)gps.texpos_button_category_rectangle);
	n->set_anchors_preset(widgets::LayoutPreset::FULL);
	std::weak_ptr<nineslice> n_wk=n;
	select_callback[row_idx]=f;
	new_row->set_custom_activated([this, row_idx, f](widgets::widget *w) {
		this->set_selected((int32_t)row_idx);
		return true;
		});
	if (row_idx==0)
		{
		set_selected(0);
		}
	return new_row;
	}

void radio_rows::rename_entry(const string &original,const string &s) {
	for (auto &child:rows.children)
		{
		auto row=std::static_pointer_cast<container>(child);
		auto t=row->add_or_get_widget<text>("Text",s);
		if (t->get_text() == original)
			{
			t->set_text(s);
			return;
			}
		}
	}

void radio_rows::set_selected(void *w) {
	for (auto i=0; i<rows.children.size(); ++i)
		{
		if (rows.children[i].get()==w)
			{
			set_selected(i);
			return;
			}
		}
	}

void radio_rows::set_selected(int32_t i) {
	selected_idx=max(0,min((int32_t)(rows.children.size())-1,i));
	selected=rows.children[selected_idx].get();
	if (auto f=select_callback.find(selected_idx); f != select_callback.end())
		{
		f->second(selected);
		}
	rows.scroll_to(selected_idx);
	refresh_selected();
	}

void radio_rows::refresh_selected() {
	for (auto i=0;i<rows.children.size();++i)
		{
		auto &child=rows.children[i];
		if (auto c=std::dynamic_pointer_cast<container>(child); c)
			{
			const auto slice=std::dynamic_pointer_cast<nineslice>(c->get_by_name("Slice"));
			if (slice)
				{
				if (child.get()==selected)
					{
					selected_idx=i;
					slice->set_texpos((int32_t *)(gps.texpos_button_category_rectangle_selected));
					}
				else
					{
					slice->set_texpos((int32_t *)(gps.texpos_button_category_rectangle));
					}
				}
			}
		}
	}

void radio_rows::feed(std::set<InterfaceKey> &events) {
	if(can_key_activate() && standardscroll(events,selected_idx,rows.page_size())) set_selected(selected_idx);
	widget::feed(events);
	rows.feed(events);
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
	key_row=key_col=0;
	key_display=add_widget<widget>();
//	key_display->set_custom_render([](widgets::widget *w) {}); // TODO: some display of some sort
	key_display->set_visible(false);
	}

void table::feed(set<InterfaceKey> &events)
	{
	if (!entries->children.empty() && keyboard_controlled)
		{
		if (events.contains(INTERFACEKEY_STANDARDSCROLL_RIGHT))
			{
			key_col=min(key_col+1,(int32_t)labels->children.size()-1);
			key_display->set_visible(true);
			events.erase(INTERFACEKEY_STANDARDSCROLL_RIGHT);
			}
		if (events.contains(INTERFACEKEY_STANDARDSCROLL_LEFT))
			{
			key_col=max(key_col-1,0);
			key_display->set_visible(true);
			events.erase(INTERFACEKEY_STANDARDSCROLL_LEFT);
			}
		if (standardscroll(events,key_row,entries->page_size()))
			{
			key_row=min(key_row,(int32_t)(entries->children.size())-1);
			key_row=max(key_row,0);
			key_display->set_visible(true);
			}
		auto &w=entries->children[key_row];
		if (auto c=std::dynamic_pointer_cast<container>(w); c)
			{
			key_display->set_parent(c->children[key_col].get());
			}
		else
			{
			key_display->set_parent(w.get());
			}
		key_display->move_to_anchor();
		if (events.contains(INTERFACEKEY_SELECT))
			{
			
			if (w->activate())
				{
				events.erase(INTERFACEKEY_SELECT);
				}
			else if (auto c=std::dynamic_pointer_cast<container>(w); c && c->children[key_col]->activate())
				{
				events.erase(INTERFACEKEY_SELECT);
				}
			}
		}
	container::feed(events);
	}

void table::arrange()
	{
	if (!labels||!entries) return;
	if (children.empty()) // can happen if cleaned
	{
		add_widget(labels); // these won't be deleted, since they still have a strong ref
		add_widget(entries);
	}
	move_to_anchor();
	for (auto i=0; i<labels->children.size(); ++i)
		{
		int this_w=labels->children[i]->min_w;
		for (auto &container_widget:entries->children)
			{
			auto entry=std::dynamic_pointer_cast<container>(container_widget);
			if (entry)
				{
				entry->arrange();
				if (entry->children.size()>i) this_w=max(this_w,entry->children[i]->width());
				}
			}
		labels->children[i]->min_w=this_w;
		}
	labels->arrange();
	int max_x=0;
	for (auto &container_widget:entries->children)
		{
		max_x=max(container_widget->width(),max_x);
		auto entry=std::dynamic_pointer_cast<container>(container_widget);
		if (entry)
			{
			for (auto i=0; i<labels->children.size(); ++i)
				{
				if (entry->children.size()<=i) continue;
				entry->children[i]->offset_left=labels->children[i]->offset_left;
				auto cur_x=entry->children[i]->width()+entry->children[i]->offset_left;
				max_x=max(cur_x,max_x);
				}
			}
		}
	for (auto &widget:entries->children)
		{
		widget->min_w=max_x;
		}
	min_w=max(max_x, min_w);
	entries->min_w=max(max_x, min_w);
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
	if (cur_idx<children.size())
		{
		children[cur_idx]->feed(events);
		}
	if (tab_labels.size()==0)
		{
		return;
		}
	if (events.contains(INTERFACEKEY_CHANGETAB))
		{
		if(switch_tab((cur_idx+1)%children.size())) events.erase(INTERFACEKEY_CHANGETAB);
		}
	if (events.contains(INTERFACEKEY_SEC_CHANGETAB))
		{
		if(cur_idx>0 && switch_tab(cur_idx-1)) events.erase(INTERFACEKEY_SEC_CHANGETAB);
		}
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
				if (i<children.size()&&!(children[i]->flag&WIDGET_VISIBILITY_ACTIVE))
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

void tabs::render(uint32_t curtick) {
	widget::render(curtick);
    int32_t tab_sx, tab_ex,tab_sy;
    tab_sx = rect.sx;
	tab_sy=rect.sy;
	std::array<std::array<std::array<int32_t,2>,5>,2> tab_pair;
	switch (tab_type)
		{
		default: // tabs::TabType::DEFAULT, but don't want potential uninitialized values or similar mucking things up
			tab_pair={ init.texpos_tab_unselected, init.texpos_tab_selected };
			break;
		case tabs::TabType::SHORT:
			tab_pair={ gps.texpos_short_tab, gps.texpos_short_tab_selected };
			break;
		case tabs::TabType::SHORT_SUB:
			tab_pair={ gps.texpos_short_subtab, gps.texpos_short_subtab_selected };
			break;
		case tabs::TabType::SHORT_SUBSUB:
			tab_pair={ gps.texpos_short_subsubtab, gps.texpos_short_subsubtab_selected };
			break;
		}
	for (auto &row:rows)
		{
		for (auto i=row.first; i<=row.second; ++i)
			{
			tab_ex=tab_sx+(int32_t)tab_labels[i].length()+3;
			auto texpos=tab_pair[cur_idx==i?1:0];
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
				is_active=!!(children[i]->flag&WIDGET_VISIBILITY_ACTIVE);
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
		{
		children[cur_idx]->render(curtick);
		}
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
	if (custom_activated.empty())
		{
		custom_activated.emplace_back([this](widgets::widget *_) {
			this->callback();
			return true;
			});
		}
	widget::feed(events);
}
void better_button::render(uint32_t curtick) {
	widget::render(curtick);
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

void interface_main_button::render(uint32_t curtick)
	{
	widget::render(curtick);
	int32_t sy,sx,ey,ex;
	coords(sy,sx,ey,ex);
	for (int py=0; py<3; ++py)
		{
		for (int px=0; px<4; ++px)
			{
			gps.locate(py+sy,px+sx);
			gps.add_tile(gps.texpos_button_main[which_button][px][py],0);
			}
		}
	}

void interface_small_button::render(uint32_t curtick)
{
	widget::render(curtick);
	int32_t sy,sx,ey,ex;
	coords(sy,sx,ey,ex);
	for (int py=0; py<2; ++py)
		{
		for (int px=0; px<2; ++px)
			{
			gps.locate(py+sy,px+sx);
			gps.add_tile(gps.texpos_button_small[which_button][px][py],0);
			}
		}
}

void interface_pets_livestock_button::render(uint32_t curtick) {
	widget::render(curtick);
	int32_t sy,sx,ey,ex;
	coords(sy,sx,ey,ex);
	for (int py=0; py<3; ++py)
		{
		for (int px=0; px<3; ++px)
			{
			gps.locate(py+sy,px+sx);
			gps.add_tile(gps.texpos_button_pets_livestock[which_button][px][py],0);
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
    set_callback([](auto _) {});
}

void textbox::take_focus() {
	gview.cur_textbox=this;
	}
void textbox::remove_focus() {
	if (gview.cur_textbox==this) gview.cur_textbox=NULL;
	}
bool textbox::is_focused() {
	return gview.cur_textbox==this;
	}

bool textbox::do_input(set<InterfaceKey> &events) {
	if (!is_visible())
		{
		return false;
		}
	else if (standardstringentry(str,maxlen,flags,events))
		{
		callback(this);
		return true;
		}
	else
		{
		return false;
		}
	}

void textbox::feed(set<InterfaceKey> &events) {
	widget::feed(events);
	if (can_key_activate())
		{
		static std::vector<InterfaceKey> inter;
		inter.clear();
		std::set_intersection(activation_hotkeys.begin(),activation_hotkeys.end(),events.begin(),events.end(),std::back_inserter(inter));
		if (!inter.empty())
			{
			take_focus();
			for (auto &i : inter)
				{
				events.erase(i);
				}
			}
		}
	if (enabler.tracking_on && enabler.mouse_lbut)
		{
		int32_t tmx=-1,tmy=-1;
		gps.get_mouse_text_coords(tmx,tmy);
		if (inside(tmy,tmx))
			{
			take_focus();
			}
		else
			{
			remove_focus();
			}
		}
}

void textbox::render(uint32_t curtick) {
  // We need to do some kind of line-breaking for multi-line text
  // entry boxes. This shall be implemented at need, and there is none
  // yet.
	widget::render(curtick);
    int sy, sx, ey, ex;
    coords(sy, sx, ey, ex);
	switch (textbox_type)
		{
		case TextboxType::FILTER:
			{
			for (auto by=sy; by <= ey; ++by)
				{
				gps.locate(by,sx);
				gps.add_lower_tile(::init.texpos_button_filter[0][by - sy]);
				gps.locate(by,ex - 3);
				gps.add_lower_tile(::init.texpos_button_filter[2][by - sy]);
				gps.locate(by,ex - 2);
				gps.add_lower_tile(::init.texpos_button_filter[3][by - sy]);
				gps.locate(by,ex - 1);
				gps.add_lower_tile(::init.texpos_button_filter[4][by - sy]);
				gps.locate(by,ex);
				gps.add_lower_tile(::init.texpos_button_filter[5][by - sy]);
				for (auto bx=sx + 1; bx <= ex - 4; ++bx)
					{
					//text
					gps.locate(by,bx);
					gps.add_lower_tile(::init.texpos_button_filter[1][by - sy]);
					if (init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))gps.addchar(0);//clear out drawborder
					}
				}
			break;
			}
		case TextboxType::NAME:
			{
			for (auto by=sy; by <= ey; ++by)
				{
				gps.locate(by,sx);
				gps.add_lower_tile(::init.texpos_button_filter[0][by - sy]);
				gps.locate(by,ex - 3);
				gps.add_lower_tile(::init.texpos_button_filter_name[0][by - sy]);
				gps.locate(by,ex - 2);
				gps.add_lower_tile(::init.texpos_button_filter_name[1][by - sy]);
				gps.locate(by,ex - 1);
				gps.add_lower_tile(::init.texpos_button_filter_name[2][by - sy]);
				gps.locate(by,ex);
				gps.add_lower_tile(::init.texpos_button_filter_name[3][by - sy]);
				for (auto bx=sx + 1; bx <= ex - 4; ++bx)
					{
					//text
					gps.locate(by,bx);
					gps.add_lower_tile(::init.texpos_button_filter[1][by - sy]);
					if (init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))gps.addchar(0);//clear out drawborder
					}
				}
			break;
			}
		}

    gps.locate(sy + 1, sx + 2);
    gps.changecolor(fg, bg, bright);
    string display_str = str;
    if (is_focused() && curtick % 1000 < 500)display_str+="_";
    gps.addst(display_str);
    return;
}

void character::render(uint32_t curtick)
	{
	widget::render(curtick);
	gps.locate(rect.sy,rect.sx);
	gps.changecolor(fg,bg,bright);
	gps.addchar_flag(c,1,flag);
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

void text::render(uint32_t curtick) {
	widget::render(curtick);
    gps.locate(rect.sy, rect.sx);
    gps.changecolor(fg, bg, bright);
	gps.addst_flag(str, just, space, flag);
}

void text_truncated::arrange()
	{
	widget::arrange();
	auto done_flags=truncate_mode_flags;
	str=original_str;
	while (width()<str.length())
		{
		if (done_flags&TRUNCATE_MODE_ABBREVIATE)
			{
			done_flags&=~TRUNCATE_MODE_ABBREVIATE;
			abbreviate_string(str,width());
			}
		else if ((done_flags&TRUNCATE_MODE_ELLIPSES) && width()>3)
			{
			done_flags&=~TRUNCATE_MODE_ELLIPSES;
			truncate_string(str,width());
			}
		else
			{
			str.resize(width());
			}
		}
	}

void text_multiline::arrange()
	{
	move_to_anchor();
	auto w=width();
	if (w>0 && w<str.length()) min_h=((int32_t)str.length()/w)+1;
	else min_h=1;
	widget::arrange();
	}

void text_multiline::render(uint32_t curtick)
	{
	widget::render(curtick);
	separate_string(str,strs,width());
	for (int i=0; i<strs.size(); ++i)
		{
		gps.locate(rect.sy+i,rect.sx);
		gps.changecolor(fg,bg,bright);
		gps.addst_flag(strs[i], just, space, flag);
		}
	}

void nineslice::render(uint32_t curtick) {
	widget::render(curtick);
    int32_t sy, sx, ey, ex;
    coords(sy, sx, ey, ex);
    draw_nineslice(selected_texpos, sy, sx, ey, ex, flag);
}

void nineslice_horizontal::render(uint32_t curtick) {
	widget::render(curtick);
	int32_t sy,sx,ey,ex;
	coords(sy,sx,ey,ex);
	draw_horizontal_nineslice(selected_texpos,sy,sx,ey,ex);
	}

void widgets::display_picture_box(widget *w) {
	int32_t sy,sx,ey,ex;
	w->coords(sy,sx,ey,ex);
	for (auto py=sy; py<sy+3; ++py)
		{
		const auto n=py-sy;
		auto px=sx;
		gps.locate(py,px);
		gps.add_lower_tile(gps.texpos_button_picture_box[0][n]);
		for (px=sx+1; px<=sx+3; ++px)
			{
			gps.locate(py,px);
			gps.add_lower_tile(gps.texpos_button_picture_box[1][n]);
			}
		gps.locate(py,sx+4);
		gps.add_lower_tile(gps.texpos_button_picture_box[2][n]);
		for (px=sx+5; px<ex; ++px)
			{
			gps.locate(py,px);
			gps.add_lower_tile(gps.texpos_button_rectangle[2][n]);
			}
		gps.locate(py,ex);
		gps.add_lower_tile(gps.texpos_button_rectangle[1][n]);
		/*
		if(selected[i])
			{
			if(px==list_sx)gps.add_lower_tile(gps.texpos_button_picture_box_highlighted[0][py-y]);
			else if(px==list_sx+1)gps.add_lower_tile(gps.texpos_button_picture_box_highlighted[1][py-y]);
			else if(px==list_sx+2)gps.add_lower_tile(gps.texpos_button_picture_box_highlighted[1][py-y]);
			else if(px==list_sx+3)gps.add_lower_tile(gps.texpos_button_picture_box_highlighted[1][py-y]);
			else if(px==list_sx+4)gps.add_lower_tile(gps.texpos_button_picture_box_highlighted[2][py-y]);
			//else if(px==list_sx+5)gps.add_lower_tile(gps.texpos_button_rectangle[0][py-y]);
			else if(px==list_ex)gps.add_lower_tile(gps.texpos_button_rectangle[2][py-y]);
			else gps.add_lower_tile(gps.texpos_button_rectangle[1][py-y]);
			}
		else
			{*/
			//}
		if (::init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))gps.addchar(0);
		}
	}

void anchored_tile::render(uint32_t curtick){
	widget::render(curtick);
	int32_t sy,sx,ey,ex;
	coords(sy,sx,ey,ex);
	locate();
	gps.add_anchored_tile(texp,offset_x,offset_y,ex-sx,ey-sy,use_color);
	}

void keybinding_display::arrange() {
	min_w=(int32_t)enabler.GetKeyDisplay(binding).size();
	widget::arrange();
	}

void keybinding_display::render(uint32_t curtick) {
	widget::render(curtick);
	locate();
	gview.print_interface_token(binding);
	}

std::shared_ptr<widget> graphics_switcher::current_widget()
	{
	if (::init.display.flag.has_flag(INIT_DISPLAY_FLAG_USE_GRAPHICS))
		{
		return graphics_widget;
		}
	else
		{
		return ascii_widget;
		}
	}

dropdown::dropdown() : widget() {
    cur_selected = 0;
	min_h=3;
    set_callback([](auto i, auto j) {});
}

dropdown::dropdown(std::initializer_list<string> new_options) : widget() {
    cur_selected = 0;
    min_h = 3;
	options.resize(new_options.size());
    std::copy(new_options.begin(), new_options.end(), options.begin());
    for (auto& s : options) {
        min_w = max(min_w, (int32_t)s.length() + 4);
    }
    set_callback([](auto i, auto j) {});
}

void dropdown::add_option(std::string& s) {
    options.push_back(s);
    min_w = max(min_w, (int32_t)s.length() + 4);
}

void dropdown::select_option(int32_t i) {
	cur_selected=i;
	if (cur_selected > (int32_t)(options.size())-1)
		{
		cur_selected=(int32_t)(options.size())-1;
		}
	if (cur_selected < 0)
		{
		cur_selected=0;
		}
	callback(i, this);
	open=false;
	}

void dropdown::select_option(string &s) {
	for (auto i=0; i<options.size(); ++i)
		{
		if (options[i]==s)
			{
			select_option(i);
			return;
			}
		}
	}

void dropdown::feed(set<InterfaceKey>& events) {
	widget::feed(events);
	int32_t sx,ex,sy,ey;
	coords(sy,sx,ey,ex);
	int32_t tmx=-1,tmy=-1;
	gps.get_mouse_text_coords(tmx,tmy);
	if (enabler.mouse_lbut)
		{
		if (open)
			{
			int32_t by=sy;
			for (int i=0; i < options.size(); i++)
				{
				if (tmx>=sx+1 && tmx <= ex && tmy>=by && tmy<=by+2)
					{
					select_option(i);
					enabler.mouse_lbut=false;
					return;
					}
				by+=3;
				}
			}
		else if (inside(tmy,tmx))
			{
			open=!open;
			enabler.mouse_lbut=false;
			}
		else
			{
			open=false;
			}
		}
}

void dropdown::render(uint32_t curtick) {
	widget::render(curtick);
    int32_t sx, ex, sy, ey;
    coords(sy, sx, ey, ex);
    //draw what we have
	if (open)
		{
		int by=sy;
		for (int i=0; i < options.size(); i++)
			{
			if (i == cur_selected)
				{
				draw_nineslice(init.texpos_selected_intro_button,by,sx + 1,by + 2,ex);
				}
			else
				{
				draw_nineslice(init.texpos_unselected_intro_button,by,sx + 1,by + 2,ex);
				}
			gps.locate(by + 1,ex - (int32_t)options[i].size() - 1);
			gps.addst(options[i]);
			by+=3;
			}
		}
	else
		{
		gps.locate(sy + 1,sx);
		gps.addst(options[cur_selected]);
		// draw the button
		draw_nineslice(init.texpos_open_list_button,sy,ex - 2,ey,ex);
		}
}
