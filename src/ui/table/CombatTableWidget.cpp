#include "CombatTableWidget.hpp"

#include "AdditionalInfoData.hpp"
#include "AdditionalInfoWidget.hpp"
#include "UtilsGeneral.hpp"

#include <QApplication>
#include <QCheckBox>
#include <QKeyEvent>
#include <QLabel>
#include <QObject>
#include <QWidget>

CombatTableWidget::CombatTableWidget(std::shared_ptr<CharacterHandler> characterHandler,
                                     QWidget *                         parent) :
    QTableWidget(parent),
    m_characterHandler(characterHandler)
{
}


void
CombatTableWidget::resynchronizeCharacters()
{
    m_characterHandler->clearCharacters();

    for (auto i = 0; i < rowCount(); i++) {
        // Cell widget is a checkbox within another widget, so find the child
        auto *const checkBox = cellWidget(i, 4)->findChild<QCheckBox *>();
        auto *const additionalInfoWidget = cellWidget(i, 5)->findChild<AdditionalInfoWidget *>();

        m_characterHandler->storeCharacter(item(i, 0)->text(),
                                           item(i, 1)->text().toInt(),
                                           item(i, 2)->text().toInt(),
                                           item(i, 3)->text().toInt(),
                                           checkBox->isChecked(),
                                           additionalInfoWidget->getAdditionalInformation());
    }
}


void
CombatTableWidget::setRowAndPlayer(QLabel *roundCounterLabel, QLabel *currentPlayerLabel, int rowEntered)
{
    selectionModel()->clearSelection();

    if (rowCount() == 0) {
        currentPlayerLabel->setText(QObject::tr("Current: None"));
        roundCounterLabel->setText(QObject::tr("Round 0"));
        return;
    }

    // Setting fonts may trigger unwished item setting events, so block the signals
    blockSignals(true);
    // Reset bold text rows to standard font
    for (auto i = 0; i < rowCount(); i++) {
        if (const auto font = item(i, 0)->font(); font.bold()) {
            for (auto j = 0; j < FIRST_FOUR_COLUMNS; j++) {
                item(i, j)->setFont(font.defaultFamily());
            }
            break;
        }
    }

    // Highlight selected row with bold fonts, but ignore the last columns (widgets)
    auto font = item(rowEntered, 0)->font();
    font.setBold(true);
    for (auto j = 0; j < FIRST_FOUR_COLUMNS; j++) {
        item(rowEntered, j)->setFont(font);
    }
    blockSignals(false);

    currentPlayerLabel->setText(QObject::tr("Current: ") + item(rowEntered, 0)->text());
}


void
CombatTableWidget::setTableRowColor(bool resetColor)
{
    blockSignals(true);

    const auto color = [](bool resetColor, bool isEnemy, bool isButton) {
        // Weaker alpha value so that the status effect buttons do not stick out too much
        const auto alpha = isButton ? 10 : 60;
        if (resetColor) {
            return QApplication::palette().color(QPalette::Base);
        }
        return isEnemy ? QColor(255, 194, 10, alpha) : QColor(12, 123, 220, alpha);
    };

    for (auto i = 0; i < rowCount(); i++) {
        const auto isEnemy = cellWidget(i, COL_ENEMY)->findChild<QCheckBox *>()->isChecked();
        const auto noButtonColor = color(resetColor, isEnemy, false);

        for (auto j = 0; j < FIRST_FOUR_COLUMNS; j++) {
            item(i, j)->setBackground(noButtonColor);
        }

        QPalette palette;
        palette.setColor(QPalette::Base, noButtonColor);
        palette.setColor(QPalette::Button, resetColor ? noButtonColor : color(resetColor, isEnemy, true));
        cellWidget(i, COL_ENEMY)->setAutoFillBackground(!resetColor);
        cellWidget(i, COL_ADDITIONAL)->setAutoFillBackground(!resetColor);
        cellWidget(i, COL_ENEMY)->setPalette(palette);
        cellWidget(i, COL_ADDITIONAL)->setPalette(palette);
    }

    blockSignals(false);
}


void
CombatTableWidget::setIniColumnTooltips(bool resetToolTip)
{
    blockSignals(true);

    for (auto i = 0; i < rowCount(); i++) {
        auto toolTipString = QString();
        if (!resetToolTip) {
            const auto rolledValue = item(i, 1)->text().toInt() - item(i, 2)->text().toInt();
            toolTipString += "Calculation: Rolled Value " + QString::number(rolledValue) + ", Modifier " + item(i, 2)->text();
        }

        item(i, 1)->setToolTip(toolTipString);
    }

    blockSignals(false);
}


void
CombatTableWidget::setStatusEffectInWidget(QVector<AdditionalInfoData::StatusEffect> statusEffects, int row)
{
    auto* const additionalInfoWidget = cellWidget(row, COL_ADDITIONAL)->findChild<AdditionalInfoWidget *>();
    additionalInfoWidget->setStatusEffects(statusEffects);
}


void
CombatTableWidget::adjustStatusEffectRoundCounter(bool decrease)
{
    for (auto i = 0; i < rowCount(); i++) {
        auto* const additionalInfoWidget = cellWidget(i, COL_ADDITIONAL)->findChild<AdditionalInfoWidget *>();
        additionalInfoWidget->adjustEffectDuration(decrease);
    }
}


QVector<QVector<QVariant> >
CombatTableWidget::tableDataFromWidget()
{
    QVector<QVector<QVariant> > tableData;
    for (auto i = 0; i < rowCount(); i++) {
        QVector<QVariant> rowValues;

        for (auto j = 0; j < columnCount(); j++) {
            switch (j) {
            case COL_ENEMY:
                rowValues.push_back(cellWidget(i, j)->findChild<QCheckBox *>()->isChecked());
                break;
            case COL_ADDITIONAL:
            {
                const auto& variant = AdditionalInfoData::getAdditionalInformationVariant(
                    cellWidget(i, j)->findChild<AdditionalInfoWidget *>()->getAdditionalInformation()
                    );
                rowValues.push_back(variant);
                break;
            }
            default:
                rowValues.push_back(item(i, j)->text());
                break;
            }
        }
        tableData.push_back(rowValues);
    }

    return tableData;
}


QVector<QVector<QVariant> >
CombatTableWidget::tableDataFromCharacterVector()
{
    QVector<QVector<QVariant> > tableData;
    for (const auto& character : m_characterHandler->getCharacters()) {
        QVector<QVariant> charValues;

        const auto additionalInformationVariant = AdditionalInfoData::getAdditionalInformationVariant(character.additionalInformation);
        charValues << character.name << character.initiative << character.modifier
                   << character.hp << character.isEnemy << additionalInformationVariant;

        tableData.push_back(charValues);
    }

    return tableData;
}


unsigned int
CombatTableWidget::getHeight() const
{
    auto height = 0;
    for (int i = 0; i < rowCount(); i++) {
        height += rowHeight(i);
    }
    return height + HEIGHT_BUFFER;
}


void
CombatTableWidget::keyPressEvent(QKeyEvent *event)
{
    // Ignore arrow keys
    if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up) {
        event->ignore();
        return;
    }
    QTableWidget::keyPressEvent(event);
}
