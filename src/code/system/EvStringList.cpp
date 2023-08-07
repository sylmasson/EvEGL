
#include    <EvGUI.h>

#define     MAX_ITEMS_BLOCK_SIZE    4
#define     VALID_BOUNDARY(Index)   ((unsigned int)Index < (unsigned int)mCount)

static const char   sZeroTermination = 0;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

const char  *c_str(const String &Str)
{
  const char  *cstr = Str.c_str();

  return !cstr ? &sZeroTermination : cstr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvStringList::EvStringList(void)
{
  mCount = 0;
  mMaxItems = 0;
  mOnChange = NULL;
  mStringList = NULL;
  AutoSort = false;
  ReverseSort = false;
  CaseSensitive = false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EvStringList::~EvStringList(void)
{
  mOnChange = NULL;
  Clear();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Clear(void)
{
  for (int i = 0; i < mCount; i++)
    delete mStringList[i];

  free(mStringList);
  mStringList = NULL;
  mMaxItems = 0;
  mCount = 0;
  modified(false);
  return mCount;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Add(const char *Str)
{
  int       count = addStr(Str);

  if (count > 0)
    modified(AutoSort);

  return count;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Add(const String &Str)
{
  return Add(c_str(Str));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Add(int Count, const char *Str)
{
  int       i, count = mCount;

  for (i = 0; i < Count; i++)
    if ((count = addStr(Str)) < 0)
      break;

  if (i > 0)
    modified(AutoSort);

  return count;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Add(const char *StrList, char Delimiter)
{
  const char  *ptr;
  int         i, len, count = -1;
  String      strList = StrList;

  if ((len = strList.length()) > 0)
  {
    strList.replace(Delimiter, '\0');
    ptr = c_str(strList);
    count = mCount;

    for (i = 0; i < len; i += strlen(&ptr[i]) + 1)
      if ((count = addStr(&ptr[i])) < 0)
        break;

    if (i > 0)
      modified(AutoSort);
  }

  return count;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Add(const String &StrList, char Delimiter)
{
  return Add(c_str(StrList), Delimiter);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Add(EvStringList &StrList)
{
  int       i, count = mCount;

  for (i = 0; i < StrList.mCount; i++)
    if ((count = addStr(c_str(StrList[i]))) < 0)
      break;

  if (i > 0)
    modified(AutoSort);

  return count;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Delete(int Index, int Count)
{
  if (!VALID_BOUNDARY(Index) || Count < 0)
    return -1;

  if (Count > 0)
  {
    int     i, j;

    for (i = Index, j = 0; j < Count; j++)
    {
      delete mStringList[i];

      if (++i == mCount)
      {
        mCount = Index;
        modified(AutoSort);
        return mCount;
      }
    }

    for (i = Index, j = Index + Count; j < mCount; i++, j++)
      mStringList[i] = mStringList[j];

    mCount -= Count;
    modified(AutoSort);
  }

  return mCount;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Insert(int Index, const char *Str)
{
  int       count = insertStr(Index, Str);

  if (count > 0)
    modified(AutoSort);

  return count;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Insert(int Index, const String &Str)
{
  return Insert(Index, c_str(Str));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Insert(int Index, int Count, const char *Str)
{
  int       i, count = mCount;

  for (i = 0; i < Count; i++)
    if ((count = insertStr(Index, Str)) < 0)
      break;

  if (i > 0)
    modified(AutoSort);

  return count;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::IndexOf(const char *Str)
{
  if (Str != NULL)
    for (int i = 0; i < mCount; i++)
      if ((!CaseSensitive ? stricmp(Str, c_str(*mStringList[i])) : strcmp(Str, c_str(*mStringList[i]))) == 0)
        return i;

  return -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::IndexOf(const String &Str)
{
  return IndexOf(c_str(Str));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Move(int Index, int NewIndex)
{
  if (!VALID_BOUNDARY(Index) || !VALID_BOUNDARY(NewIndex))
    return -1;

  if (Index != NewIndex)
  {
    int     i;
    String  *tmp;

    if (Index < NewIndex)
      for (tmp = mStringList[i  = Index]; i < NewIndex; i++)
        mStringList[i] = mStringList[i+1];

    else
      for (tmp = mStringList[i  = Index]; i > NewIndex; i--)
        mStringList[i] = mStringList[i-1];

    mStringList[i] = tmp;
    modified(false);
  }

  return mCount;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Set(int Index, const char *Str)
{
  if (!Str || !VALID_BOUNDARY(Index))
    return -1;

  *mStringList[Index] = Str;
  modified(false);
  return mCount;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Set(int Index, const String &Str)
{
  return Set(Index, c_str(Str));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::Sort(void)
{
  if (mCount > 0)
    modified(true);

  return mCount;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String      &EvStringList::Get(int Index)
{
  return operator[](Index);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

String      &EvStringList::operator[](const int Index)
{
  if (!VALID_BOUNDARY(Index))
  {
    char    str[100];
    static  String dummy;

    snprintf(str, sizeof(str) - 1, "EvStringList(%p)::operator[%d] Error: Out of boundary.", this, Index);
    Serial.println(str);
    return dummy = "";    // dummy string is return to prevents crash if Index is out of boundary
  }

  return *mStringList[Index];
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvStringList::SetOnChange(void (*OnChange)(EvObj *Sender, int32_t Count), EvObj *Sender)
{
  mOnChange = OnChange;
  mSender = Sender;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvStringList::Modified(void)
{
  if (mOnChange != NULL)
    (*mOnChange)(mSender, mCount);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvStringList::modified(bool Sort)
{
  if (Sort && mCount > 0)
    sortStringList();

  Modified();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::addStr(const char *Str)
{
  String    *newStr;

  if (mCount >= mMaxItems)
    resizeStringList(mMaxItems + MAX_ITEMS_BLOCK_SIZE);

  if (mCount >= mMaxItems || (newStr = new String(Str)) == NULL)
    return -1;

  mStringList[mCount] = newStr;
  return ++mCount;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::insertStr(int Index, const char *Str)
{
  String    *newStr;

  if (Index == mCount)
    return addStr(Str);

  if (mCount >= mMaxItems)
    resizeStringList(mMaxItems + MAX_ITEMS_BLOCK_SIZE);

  if (mCount >= mMaxItems || !VALID_BOUNDARY(Index) || (newStr = new String(Str)) == NULL)
    return -1;

  for (int i = mCount; i > Index; i--)
    mStringList[i] = mStringList[i-1];

  mStringList[Index] = newStr;
  return ++mCount;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void        EvStringList::resizeStringList(int NewMaxItems)
{
  void      *ptr;

  if ((ptr = realloc(mStringList, NewMaxItems * sizeof(mStringList[0]))) != NULL)
  {
    mStringList = (String **)ptr;
    mMaxItems = NewMaxItems;
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::sortStringList(void)
{
  qsort(&mStringList[0], mCount, sizeof(String **), !CaseSensitive ? sStrCaseCmp : sStrCmp);

  if (ReverseSort)
  {
    int     i, j;
    String  *tmp;

    for (i = 0, j = mCount - 1; i < j; i++, j--)
    {
      tmp = mStringList[i];
      mStringList[i] = mStringList[j];
      mStringList[j] = tmp;
    }
  }

  return mCount;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::sStrCmp(const void *Str1, const void *Str2)
{
  const char *s1 = c_str(**((String **)Str1));
  const char *s2 = c_str(**((String **)Str2));

  return strcmp(s1, s2);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int         EvStringList::sStrCaseCmp(const void *Str1, const void *Str2)
{
  const char *s1 = c_str(**((String **)Str1));
  const char *s2 = c_str(**((String **)Str2));

  return stricmp(s1, s2);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
