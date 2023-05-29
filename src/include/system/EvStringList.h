
#ifndef     _EV_STRINGLIST_H_
#define     _EV_STRINGLIST_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class EvStringList
{
  public:
    EvStringList(void);
    ~EvStringList(void);

    int           Clear(void);
    int           Add(const char *Str);
    int           Add(const String &Str);
    int           Add(int Count, const char *Str = "");
    int           Add(const char *StrList, char Delimiter);
    int           Add(const String &StrList, char Delimiter);
    int           Add(EvStringList &StrList);
    int           Delete(int Index, int Count = 1);
    int           Insert(int Index, const char *Str);
    int           Insert(int Index, const String &Str);
    int           Insert(int Index, int Count = 1, const char *Str = "");
    int           IndexOf(const char *Str);
    int           IndexOf(const String &Str);
    int           Move(int Index, int NewIndex);
    int           Set(int Index, const char *Str);
    int           Set(int Index, const String &Str);
    int           Sort(void);
    String        &Get(int Index);
    String        &operator[](const int Index);         // no modified call
    void          SetOnChange(void (*OnChange)(EvObj *Sender, int16_t Count), EvObj *Sender);
    void          Modified(void);

  protected:
    int16_t       mCount;
    int16_t       mMaxItems;
    EvObj         *mSender;
    String        **mStringList;

  private:
    void          (*mOnChange)(EvObj *Sender, int16_t Count);

    void          modified(bool Sort);
    int           addStr(const char *Str);
    int           insertStr(int Index, const char *Str);
    void          resizeStringList(int NewMaxItems);
    int           sortStringList(void);

    static int    sStrCmp(const void *Str1, const void *Str2);
    static int    sStrCaseCmp(const void *Str1, const void *Str2);

  public:
    bool          AutoSort;   // Auto sort after Add, Insert and Delete method
    bool          ReverseSort;
    bool          CaseSensitive;

    const int16_t &Count = mCount;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern const char *c_str(const String &Str);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif  /* _EV_STRINGLIST_H_ */
