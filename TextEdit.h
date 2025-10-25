#pragma once

#include <QPlainTextEdit>

#include "PostProcessExecutor.h"
#include "PunOptional.h"

class TextEdit
: public QPlainTextEdit
{

public:
	TextEdit(QWidget *parent = nullptr);

	PostProcessExecutor* PostProcess();
	pun_optional_t<std::pair<uint32_t, int>> GetBlockPosition(const QPoint& globalPos) const;

private:
	PostProcessExecutor* m_postProcess{ nullptr };
};

