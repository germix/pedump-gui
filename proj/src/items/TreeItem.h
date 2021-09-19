#ifndef TREEITEM_H
#define TREEITEM_H
#include <QIcon>
#include <QString>
#include <QVector>

class QWidget;

class TreeItem
{
public:
	TreeItem* parent;
	QVector<TreeItem*> children;
public:
	TreeItem(TreeItem* parent);
	virtual ~TreeItem();
public:
	virtual QIcon getIcon() const
	{
		return QIcon();
	}
	virtual QString getLabel() const
	{
		return QString();
	}
	virtual QWidget* getWidget() const
	{
		return nullptr;
	}
};

QWidget* createTextEdit(const QString& text);

#endif // TREEITEM_H
