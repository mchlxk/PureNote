
#include <QSyntaxHighlighter>

#include "PostProcessBlock.h"
#include "PostProcessSlot.h"

class PostProcessExecutor
: public QSyntaxHighlighter
{

public:
	PostProcessExecutor(QTextDocument* parent = nullptr);

	void AppendSlot(post_process_slot_t slot);

private slots:
	void at_postProcessBlock_aboutToBeDestroyed(uint32_t id);

protected:
	void highlightBlock(const QString& text) override;

private:
	std::vector<post_process_slot_t> m_postProcessStack;

	void ProcessPerSlot(post_process_slot_t& slot, uint32_t blockId, const QString& blockText);
};

