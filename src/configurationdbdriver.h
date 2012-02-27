#ifndef CONFIGURATIONDBDRIVER_H
#define CONFIGURATIONDBDRIVER_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMutex>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QVariant>

class ConfigurationDBDriverPrivate;
class ConfigurationDBDriver : public QObject
{
    Q_OBJECT
	static ConfigurationDBDriverPrivate* m_Instance;

	explicit ConfigurationDBDriver(QObject *parent = 0) : QObject(parent) {}
public:
	static ConfigurationDBDriver* instance() {
		static QMutex mutex;
		QMutexLocker locker(&mutex);
		if (!m_Instance) {
				m_Instance = new ConfigurationDBDriverPrivate();
		}
		return m_Instance;
	}

	static void drop() {
		static QMutex mutex;
		QMutexLocker locker(&mutex);
		delete m_Instance;
		m_Instance = 0;
		qDebug() << Q_FUNC_INFO << "droppped";
	}
};

class ConfigurationDBDriverPrivate: public QObject
{
	Q_OBJECT
	enum DROPBOX_DB_VERSION {DROPBOX_DB, CONFIG_DB, UNKNOWN} dbVersion;
	Q_ENUMS(DROPBOX_DB_VERSION)
	QSqlDatabase* db;
public:
	explicit ConfigurationDBDriver(QObject *parent = 0);

	~ConfigurationDBDriverPrivate();

	QStringList listKeys() const;

	bool hasKey(const QString& key);
	QVariant getValue(const QString& key, QVariant defaultValue=QVariant());
	void setValue(const QString &key, const QVariant& value);
	void deleteValue(const QString& key);

private:
	inline QSqlQuery justQuery(const QString& query)
	{
		if(dbVersion != CONFIG_DB)
			return QSqlQuery();

		QSqlQuery result = db->exec(query);
		if(db->lastError().isValid())
			qDebug() << db->lastError() << "\nquery:" << query;
		return result;
	}
};

#endif // CONFIGURATIONDBDRIVER_H
