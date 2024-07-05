#ifndef ALLOW_H
#define ALLOW_H

#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <QFile>
#include <QTextStream>

#include <QDebug>
#include <QString>

#include "denieditem.h"

#define TEXT_SIZE 3

using namespace std;

class Allow
{
private:
    /* data */
    vector<DeniedItem> allows;

    QString filePath;

    QFile mFile;

    bool mOpenSuccess;

public:
    Allow(QString path)
        : filePath(path)
        , mOpenSuccess(false)
    {
        mFile.setFileName(path);
        qDebug() << "Allow()";
    }

    ~Allow()
    {
        if (mOpenSuccess) {
            mFile.close();
        }
        qDebug() << "~Allow()";
    }

    /**
     * @brief 按行读取，识别出scontext tcontext tclass denied
     *
     */
    int filterFileByLine()
    {
        if (!mFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "open file fail!" << mFile.errorString();
            mOpenSuccess = false;
            return -1;
        }
        mOpenSuccess = true;

        QTextStream in(&mFile);
        in.setEncoding(QStringConverter::Utf8);

        while (!in.atEnd())
        {
            QString qline = in.readLine();
            string line = qline.toStdString();
            int r = line.find("avc: ");
            if (r != string::npos)
            {
                //开始处理
                DeniedItem item;

                // deind
                // type=1400 audit(0.0:536): avc: denied { sys_ptrace } for capability=19
                // scontext=u:r:life_cycle_hal:s0 tcontext=u:r:life_cycle_hal:s0
                // tclass=capability permissive=0
                {
                    int pre = line.find_first_of("{");
                    int last = line.find_first_of("}");

                    string t_deind = line.substr(pre + 2, last - pre - 3);

                    stringSplit(t_deind, ' ', item.denied);
                }

                // scontext tcontext tclass
                {
                    vector<string> contexts;
                    string temp("scontext=");
                    int pos_scontext = line.find(temp);
                    if (pos_scontext == string::npos) {
                        continue;
                    }

                    string t_scontext = line.substr(pos_scontext);
                    stringSplit(t_scontext, ' ', contexts);

                    for (int i = 0; i < contexts.size(); i++)
                    {
                        if (i == 0)
                        {
                            vector<string> temps;

                            stringSplit(contexts[i], ':', temps);

                            item.scontext = temps[2];
                        }
                        else if (i == 1)
                        {
                            vector<string> temps;

                            stringSplit(contexts[i], ':', temps);

                            item.tcontext = temps[2];
                        }
                        else if (i == 2)
                        {
                            int pos = contexts[i].find("=");

                            string tclass = contexts[i].substr(pos + 1);
                            item.tclass = tclass;
                        }
                        else
                        {
                        }
                    }
                }

                allows.push_back(item);
            }
        }
        return allows.size();
    }

    /**
     * @brief 打印allow结果到std::out
     *
     */
    void printAllow(vector<string>& out)
    {
        vector<DeniedItem> result;
        removeDuplicate(result);

        for (auto &item : result)
        {

            string denied("");

            if (item.denied.size() < 2)
            {
                denied = item.denied[0];
            }
            else
            {
                for (int i = 0; i < item.denied.size(); i++)
                {
                    denied += item.denied[i] + " ";
                }
                denied = "{ " + denied + "}";
            }
            string _allow = "<font size=5 color=green>allow </font>";
            string _scontext = "<font size=5 color=#1E90FF> " + item.scontext + " </font>";
            string _tcontext = "<font size=5 color=#B8860B> " + item.tcontext + " </font>";
            string _tclass = "<font size=5 color=#708090> " + item.tclass + " </font>";
            string _denied = "<font size=5 color=#FF0000> " + denied + " </font>";
            string _maohao = "<font size=5 color=black>:</font>";
            string _fenhao = "<font size=5 color=black>;</font>";
            string all = _allow + _scontext + _tcontext + _maohao + _tclass + _denied + _fenhao;
            out.push_back(all);
            // ss << "allow " << item.scontext << " " << item.tcontext << ":" << item.tclass << " " << denied << ";";
        }
        std::sort(out.begin(), out.end());
    }

    /**
     * @brief 合并denied，去除重复的条目
     *
     * @param result [out]
     */
    void removeDuplicate(vector<DeniedItem> &result)
    {
        for (auto &item : allows)
        {
            auto ret = std::find(result.begin(), result.end(), item);
            if (ret == result.end()) {
                result.push_back(item);
            } else {
                // 利用set集合的特性，去重
                ret->denied.insert(ret->denied.end(), item.denied.begin(), item.denied.end());
                std::set<std::string> order(ret->denied.begin(), ret->denied.end());
                ret->denied.assign(order.begin(), order.end());
            }
        }
    }

    /**
     * @brief 分割字符串
     *
     * @param str [in]
     * @param split [char]
     * @param rst [out]结果
     */
    void stringSplit(string str, const char split, vector<string> &rst)
    {
        istringstream iss(str);            // 输入流
        string token;                      // 接收缓冲区
        while (getline(iss, token, split)) // 以split为分隔符
        {
            // cout << "split: " << token << endl; // 输出
            rst.push_back(token);
        }
    }
};

#endif // ALLOW_H
