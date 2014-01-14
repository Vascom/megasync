#include "ExportProcessor.h"

ExportProcessor::ExportProcessor(MegaApi *megaApi, QStringList fileList) :
    QTMegaRequestListener()
{
    this->megaApi = megaApi;
    this->fileList = fileList;

    currentIndex = 0;
    remainingNodes = fileList.size();
    importSuccess = 0;
    importFailed = 0;
}

void ExportProcessor::requestLinks()
{
    for(int i=0; i<fileList.size(); i++)
    {
#ifdef WIN32
        string tmpPath((const char*)fileList[i].utf16(), fileList[i].size()*sizeof(wchar_t));
#else
        string tmpPath((const char*)fileList[i].toUtf8().constData());
#endif
        Node *node = megaApi->getSyncedNode(&tmpPath);
        megaApi->exportNode(node, this);
    }
}

QStringList ExportProcessor::getValidLinks()
{
    return validPublicLinks;
}

void ExportProcessor::QTonRequestFinish(MegaApi *api, MegaRequest *request, MegaError *e)
{
    currentIndex++;
    remainingNodes--;
    if(e->getErrorCode() != API_OK)
    {
        publicLinks.append(QString());
        importFailed++;
    }
    else
    {
        publicLinks.append(QString::fromAscii(request->getLink()));
        validPublicLinks.append(QString::fromAscii(request->getLink()));
        importSuccess++;
    }

    if(!remainingNodes)
    {
        emit onRequestLinksFinished();
    }
}