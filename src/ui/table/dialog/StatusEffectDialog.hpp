#pragma once

#include <QDialog>
#include <QPointer>

class QListWidget;

// Dialog used to add certain status effects to characters
class StatusEffectDialog : public QDialog {
	Q_OBJECT

public:
	explicit
	StatusEffectDialog(QWidget *parent = 0);

	[[nodiscard]] QString
	getEffect() const
	{
		return m_effect;
	}

private slots:

	void
	addEffect();

private:

	void
	findEffect(const QString filter);

	QPointer<QListWidget> m_list;

	// Status Effects are based on the Pathfinder 1e rulebook
	QStringList m_effects = {
		tr("Bleed"),
		tr("Blinded"),
		tr("Confused"),
		tr("Cowering"),
		tr("Concealed"),
		tr("Damage Reduction"),
		tr("Dazed"),
		tr("Dazzled"),
		tr("Disabled"),
		tr("Deafened"),
		tr("Dying"),
		tr("Energy Drained"),
		tr("Energy Resistance"),
		tr("Entangled"),
		tr("Exhausted"),
		tr("Fascinated"),
		tr("Fatigued"),
		tr("Flat-footed"),
		tr("Frightened"),
		tr("Grappled"),
		tr("Haste"),
		tr("Helpless"),
		tr("Incorporeal"),
		tr("Invisible"),
		tr("Nauseated"),
		tr("Panicked"),
		tr("Paralyzed"),
		tr("Petrified"),
		tr("Pinned"),
		tr("Prone"),
		tr("Shaken"),
		tr("Sickened"),
		tr("Spell Resistence"),
		tr("Stable"),
		tr("Staggered"),
		tr("Stunned"),
		tr("Unconscious")
	};

	QString m_effect;
};
