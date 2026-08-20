#include "utils/common_includes.h"
#include "reports/eossdk_reports.h"
#include "eossdk_platform.h"

EOS_DECLARE_FUNC(void) EOS_Reports_SendPlayerBehaviorReport(EOS_HReports Handle, const EOS_Reports_SendPlayerBehaviorReportOptions* Options, void* ClientData, const EOS_Reports_OnSendPlayerBehaviorReportCompleteCallback CompletionDelegate)
{ GetEOS_Reports().SendPlayerBehaviorReport(Options, ClientData, CompletionDelegate); }
