function isLeapYear(year) {
    return (year%4==0&&((year%100!=0)||(year%400==0)));
}

function getMaxDate(year,month)
{
    if(month===1||month===3||month===5||month===7||month===8||month===10||month===12)
    {
        return 31
    }
    else if(month===2)
    {
        if(isLeapYear(year))
        {
            return 29
        }
        else
        {
            return 28
        }
    }
    else
    {
        return 30

    }
}

function getDataFromStr(str,format,index)
{
    if(format==="yyyy/MM/dd"||format==="yyyy/M/d")
    {
        var curArray=  str.split("/")
       if(curArray.length>index)
       {
        return curArray[index]
       }
       else
       {
         return ""
       }
    }
    else if(format==="yyyy-MM-dd"||format==="yyyy-M-d")
    {
        var array=  str.split("-")
        if(array.length>index)
        {
           return array[index]
        }
        else
        {
          return ""
        }


    }
    else
    {
        return ""
    }

}
