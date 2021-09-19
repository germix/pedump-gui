#include "TreeItem.h"

TreeItem::TreeItem(TreeItem* ___parent)
	: parent(___parent)
{
	if(parent)
		parent->children.append(this);
}

TreeItem::~TreeItem()
{
	//
	// Remover del padre
	//
	if(parent)
	{
		parent->children.removeOne(this);
	}
	//
	// Eliminar hijos
	//
	for(int i = 0; i < children.count(); i++)
	{
		TreeItem* child = children.at(i);
		// ...
		child->parent = nullptr;
		// ...
		delete child;
	}
	children.clear();
}

#include <QTextEdit>

QWidget* createTextEdit(const QString& text)
{
	QTextEdit* edit = new QTextEdit();
	edit->setFont(QFont("Courier New"));
	edit->setText(text);
	edit->setReadOnly(true);
	if(text.isEmpty())
	{
		edit->setDisabled(true);
	}
	return edit;
}
