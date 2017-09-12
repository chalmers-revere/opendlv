/**
 * Copyright (C) 2017 Chalmers Revere
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WPushButton>

#include <opendavinci/odcore/base/Lock.h>

#include "livefeedapp.hpp"

namespace opendlv {
namespace gui {

LiveFeedApp::LiveFeedApp(Wt::WEnvironment const &a_environment)
    : Wt::WApplication(a_environment),
    m_container(),
    m_mutex(),
    m_nameEdit(),
    m_greeting(),
    m_isInitialized(false)
{
  setTitle("Hello world");

  root()->addWidget(new Wt::WText("Your name, please? "));
  m_nameEdit = new Wt::WLineEdit(root());
  m_nameEdit->setFocus();

  Wt::WPushButton *button = new Wt::WPushButton("Greet me", root());
  button->setMargin(5, Wt::Left);

  root()->addWidget(new Wt::WBreak());

  m_greeting = new Wt::WText(root());

  button->clicked().connect(std::bind([&]() { 
        Greet();
  }));

  m_isInitialized = true;
}

LiveFeedApp::~LiveFeedApp() 
{
}

void LiveFeedApp::AddContainer(odcore::data::Container &a_container)
{
  if (m_isInitialized) {
    odcore::base::Lock l(m_mutex);
    m_container = a_container;
   // std::cout << "Added " << 
  }
}

void LiveFeedApp::Greet() 
{
  odcore::base::Lock l(m_mutex);
  m_greeting->setText(m_container.toString());
}

}
}
