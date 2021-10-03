#pragma once

#include "../CharacterHandler.hpp"

#include <QLineEdit>
#include <QPointer>
#include <QWidget>

class QCheckBox;
class QLineEdit;
class QPushButton;
class QSpinBox;

// This class handles the widget used for the creation of characters
class CharacterCreationWidget : public QWidget {
	Q_OBJECT

public:
	CharacterCreationWidget(CharacterHandlerRef	charSort,
				bool			isEditCreation,
				QWidget *		parent = 0);

	// Store the last created character before opening the table widget
	void
	storeLastCharacter();

	// Returns if a name has been entered
	bool
	isNameEmpty()
	{
		return m_nameEdit->text().isEmpty();
	}

	// Set the focus to the name label at the start of the character creation
	void
	setNameFocus()
	{
		m_nameEdit->setFocus(Qt::TabFocusReason);
	}

signals:

	void
	cancel();

	void
	finish();

private slots:

	void
	saveAndCreateNewCharacter();

	void
	resetCharacter();

private:

	void
	storeCharacter();

	// Widgets
	QPointer<QLineEdit> m_nameEdit;
	QPointer<QSpinBox> m_initBox;
	QPointer<QSpinBox> m_initModifierBox;
	QPointer<QSpinBox> m_hpBox;
	QPointer<QCheckBox> m_isNPCBox;
	QPointer<QLineEdit> m_addInfoEdit;

	QPointer<QPushButton> m_saveButton;
	QPointer<QPushButton> m_finishButton;
	QPointer<QPushButton> m_resetButton;
	QPointer<QPushButton> m_cancelButton;

	bool m_isEditCreation;

	// character sort object to store new created objects
	CharacterHandlerRef m_char;
};
