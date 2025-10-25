
#include <QSyntaxHighlighter>

#include "PostProcessBlock.h"
#include "PostProcessPass.h"

class PostProcessExecutor
: public QSyntaxHighlighter
{

public:
	PostProcessExecutor(QTextDocument* parent = nullptr);

	void AddPass(post_process_pass_t pass);

private slots:
	void at_postProcessBlock_aboutToBeDestroyed(uint32_t id);

protected:
	void highlightBlock(const QString& text) override;

private:
	std::vector<post_process_pass_t> m_postProcessStack;

	void ExecutePass(post_process_pass_t& ppp, uint32_t blockId, const QString& blockText);
};

