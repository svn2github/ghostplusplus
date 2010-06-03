#include "widget.h"

#include "layout.h"

CWidget::CWidget(CWidget *parent, bool dummy)
{
	_parent = parent;
	_layout = 0;

	_topMargin = 1;
	_bottomMargin = 1;
	_leftMargin = 1;
	_rightMargin = 1;
	_selected = false;
	_visible = true;

	// If we haven't initialized curses yet, don't create a new curses window and panel
	if(!dummy)
	{
		_window = newwin(1, 1, 0, 0);
		_panel = new_panel(_window);
	}
	else
	{
		_window = 0;
		_panel = 0;
	}

	hide();
}

void CWidget::setParent(CWidget *parent)
{
	_parent = parent;
}

void CWidget::setPosition(uint x, uint y)
{
	_pos.set(x, y);

	if(_layout)
		_layout->setPosition(x, y);
}

CPoint CWidget::pos()
{
	return _pos;
}

void CWidget::setLayout(CLayout *layout)
{
	_layout = layout;
	_layout->setSize(_size.width(), _size.height());
	_layout->setPosition(_pos.x(), _pos.y());
}

void CWidget::setSize(uint width, uint height)
{
	_size.set(width, height);

	wresize(_window, height, width);
	replace_panel(_panel, _window);

	if(_layout)
		_layout->setSize(width, height);
}

void CWidget::setFixedSize(uint width, uint height)
{
	_size.set(width, height);
	_size.setFixed(true);

	wresize(_window, height, width);
	replace_panel(_panel, _window);

	if(_layout)
		_layout->setSize(width, height);
}

CSize CWidget::size()
{
	return _size;
}

void CWidget::hide()
{
	hide_panel(_panel);
	_visible = false;

	// Hide subwidgets too
	if(_layout)
		_layout->hide();
}

void CWidget::show()
{
	show_panel(_panel);
	_visible = true;

	// Show subwidgets too
	if(_layout)
		_layout->show();
}

void CWidget::update()
{
	if(_visible)
	{
		move_panel(_panel, _pos.y(), _pos.x());
		top_panel(_panel);

		for(uint i = 0; i < _size.width(); i++)
			for(uint j = 0; j < _size.height(); j++)
				waddch(_window, 'X');

		if(_layout)
			_layout->update();
	}
}
