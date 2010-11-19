#include "XmlComment.h"

#include "../../Io/Stream.h"

namespace zenic
{

XmlNode::Type XmlComment::type() const
{
	return Comment;
}

XmlNode* XmlComment::clone() const
{
	return new XmlComment;
}

bool XmlComment::write(Stream& stream, uint depth) const
{
	for (uint i = 0; i < depth; ++i)
		stream << "    ";
	stream << "<!--" << value() << "-->";

	return true;
}

}
