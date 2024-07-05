#ifndef DENIEDITEM_H
#define DENIEDITEM_H

#include <string>
#include <vector>


class DeniedItem
{
public:
    std::string scontext;
    std::string tcontext;
    std::string tclass;
    std::vector<std::string> denied;
    DeniedItem():scontext(""),tcontext(""),tclass("") {}
    DeniedItem(const DeniedItem &item)
    {
        this->scontext = item.scontext;
        this->tcontext = item.tcontext;
        this->tclass = item.tclass;
        this->denied = item.denied;
    }
    ~DeniedItem() {}
    void print();
    bool operator==(const DeniedItem &item) const {
        return (scontext == item.scontext && tcontext == item.tcontext && tclass == item.tclass);
    }
    bool operator<(const DeniedItem &item) const {
        return scontext < item.scontext;
    }
    void operator=(const DeniedItem &item) {
        this->scontext = item.scontext;
        this->tcontext = item.tcontext;
        this->tclass = item.tclass;
        this->denied = item.denied;
    }
};

#endif // DENIEDITEM_H
