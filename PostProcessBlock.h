#include <QObject>
#include <QTextBlockUserData>

class PostProcessBlock
: public QObject
, public QTextBlockUserData
{
Q_OBJECT

public:
	PostProcessBlock(QObject* parent = nullptr);

	 uint32_t GetId() const;

signals:
	void about_to_be_destroyed(uint32_t id);

private slots:
	void at_qobject_destroyed();

private:
	const uint32_t m_id;
	static uint32_t next_id;
};

