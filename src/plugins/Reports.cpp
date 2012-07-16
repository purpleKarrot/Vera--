//
// Copyright (C) 2006-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "Reports.h"
#include "Rules.h"
#include <map>
#include <utility>

namespace Vera
{
namespace Plugins
{

typedef std::multimap<int, Reports::Message> FileMessagesCollection;
typedef std::map<Reports::FileName, FileMessagesCollection> MessagesCollection;

static MessagesCollection messages_;
static bool showRules_;

void Reports::setShowRules(bool show)
{
    showRules_ = show;
}

void Reports::add(const FileName & name, int lineNumber, const Message & msg)
{
    const Rules::RuleName currentRule = Rules::getCurrentRule();

    messages_[name].insert(make_pair(lineNumber,
            showRules_ ? '(' + currentRule + ") " + msg : msg));
}

void Reports::dumpAll(std::ostream& os, bool omitDuplicates)
{
    for (MessagesCollection::iterator it = messages_.begin(), end = messages_.end();
         it != end; ++it)
    {
        const FileName & name = it->first;
        FileMessagesCollection & fileMessages = it->second;

        FileMessagesCollection::iterator fit = fileMessages.begin();
        FileMessagesCollection::iterator fend = fileMessages.end();

        int lastLineNumber = 0;
        Message lastMsg;
        for ( ; fit != fend; ++fit)
        {
            int lineNumber = fit->first;
            const Message & msg = fit->second;

            if (omitDuplicates == false || lineNumber != lastLineNumber || msg != lastMsg)
            {
                os << name << ':' << lineNumber << ": " << msg << '\n';

                lastLineNumber = lineNumber;
                lastMsg = msg;
            }
        }
    }
}

} // namespace Plugins
} // namespace Vera
