
# Contributing

## Reporting Issues

Bug reports are appreciated. Following a few guidelines listed below will help speed up the process of getting them fixed. 

1. Search the issue tracker to see if it has already been reported.
2. Include additional information such as:
    * A detailed explanation
    * OpenGD Debug-Info
     * Operating System version
    * Screen shots (if applicable)
    * ...and any other relevant information

#### GENERAL
1. ##### Do not use Java-like braces.

  * ###### Good:
    ```cpp
    void MyClass::method1()
    {
        if (aCondition)
        {
            // Do something
        }
    }
    ```

  * ###### Bad:
    ```cpp
    void MyClass::method1() {
        if (aCondition) {
            // Do something
        }
    }
    ```
  If the method requires only 1 line, Java-like braces should be used.
```cpp
int method2() {
	return _x; // only one line code can be placed in .h as method definition
};
```
3. ##### Use tabs instead of white-spaces (we usually set our editors to 4 white-spaces for 1 tab, but the choice is up to you).


4. ##### Always leave one space before and after binary and ternary operators.

  * ###### Good:
    ```cpp
    if (a == 10 && b == 42)
    ```

  * ###### Bad:
    ```cpp
    if (a==10&&b==42)
    ```

5. ##### Only leave one space after semi-colons in "for" statements.

  * ###### Good:
    ```cpp
    for (int i = 0; i != 10; ++i)
    ```

  * ###### Bad:
    ```cpp
    for(int i=0;i<10;++i)
    ```

6. ##### Function names are not separated from the first parenthesis.

  * ###### Good:
    ```cpp
    foo();
    myObject.foo(24);
    ```

  * ###### Bad:
    ```cpp
    foo ();
    ```

7. ##### Keywords are separated from the first parenthesis by one space.

  * ###### Good:
    ```cpp
    if (true)
    while (true)
    ```

  * ###### Bad:
    ```cpp
    if(myCondition)
    ```

8. ##### Use the following indenting for "switch" statements:

  ```cpp
  switch (test)
  {
      case 1:
      {
          // Do something
          break;
      }
      default:
          // Do something else
  } // No semi-colon here
  ```
  If the statements are short, consistent and straightforward prefer the following
  ```cpp
switch (n) 
{
	default: return "GJ_createBtn_001.png"; 
	case 1: return "GJ_savedBtn_001.png";
	case 2: return "GJ_highscoreBtn_001.png";
	case 3: return "GJ_challengeBtn_001.png";
	case 4: return "GJ_dailyBtn_001.png";
	case 5: return "GJ_weeklyBtn_001.png";
	case 6: return "GJ_gauntletsBtn_001.png";
	case 7: return "GJ_featuredBtn_001.png";
	case 8: return "GJ_fameBtn_001.png";
	case 9: return "GJ_mapPacksBtn_001.png";
	case 10: return "GJ_searchBtn_001.png";
}
```
9. ##### Prefer enums for integer constants.

10. ##### Use initialization with curly braces.

  * ###### Good:
    ```cpp
    MyClass instance{10.4};
    ```

  * ###### Bad:
    ```cpp
    MyClass instance(10.4);
    ```

11. ##### Always use `empty()` for testing if a string is empty or not.

  * ###### Good:
    ```cpp
    if (!string.empty())
    ...
    ```

  * ###### Bad:
    ```cpp
    if (string != "")
    ...
    ```


12. ##### Always use `C++ conversion` instead of `C-Style cast`. Generally, all the conversion among types should be avoided. If you have no choice, use C++ conversion.

  * ###### Good:
    ```cpp
    char aChar = static_cast<char>(_pEditView->execute(SCI_GETCHARAT, j));
    ```

  * ###### Bad:
    ```cpp
    char aChar = (char)_pEditView->execute(SCI_GETCHARAT, j);
    ```

13. ##### Use `!` instead of `not`, `&&` instead of `and`, `||` instead of `or`.

  * ###### Good:
    ```cpp
    if (!PathFileExists(dir2Search))
    ```

  * ###### Bad:
    ```cpp
    if (not PathFileExists(dir2Search))
    ```

14. ##### Use C++17 initialization statements for variables only used in if/for statements

  * ###### Good:
    ```cpp
    if (int temp = 5; temp != 0)
	{
		//use temp
	}
	//good: temp scope is limited to the if statement condition and block
    ```

  * ###### Bad:
    ```cpp
	int temp = 5;
    if (temp != 0)
	{
		//use temp
	}
	//bad: temp scope is not limited
    ```


15. ##### Use the pointer to bool conversion to check if a pointer is nullptr

  * ###### Good:
    ```cpp
    if (myPointer && !myOtherPointer)
	{
		//myPointer is not nullptr
		//myOtherPointer is nullptr
	}
    ```

  * ###### Bad:
    ```cpp
    if (myPointer != nullptr && myOtherPointer == nullptr)
	{
		//myPointer is not nullptr
		//myOtherPointer is nullptr
	}
    ```

#### NAMING CONVENTIONS

  * ###### Good:
    ```cpp
    if (!PathFileExists(dir2Search))
    ```

  * ###### Bad:
    ```cpp
    if (not PathFileExists(dir2Search))
    ```
1. ##### Classes uses Pascal Case

  * ###### Good:
    ```cpp
    class IAmAClass
    {};
    ```

  * ###### Bad:
    ```cpp
    class iAmAClass
    {};
    class I_am_a_class
    {};
    ```

2. ##### Methods
  ```cpp
  void myMethod(uint myVeryLongParameter);
  ```

3. ##### Member variables

Any member variable name of class/struct should be preceded by an underscore.

  ```cpp
  public:
      int _myInt;
  private:
      float _myFloat;
  ```

4. ##### Always prefer a variable name that describes what the variable is used for.

  * ###### Good:
    ```cpp
    if (hours < 24 && minutes < 60 && seconds < 60)
    ```

  * ###### Bad:
    ```cpp
    if (a < 24 && b < 60 && c < 60)
    ```



#### COMMENTS

  * ###### Good:
    ```
    // Two lines comment
    // Two lines comment
    ```

  * ###### Bad:
    ```
    /*
    Please don't piss me off with that
    */
    ```



#### BEST PRACTICES

1. ##### Use C++11/14/17 whenever it is possible

2. ##### Prefer Pre-increment:
  ```cpp
  ++i
  ```

 ##### **Over Post-increment:**
  ```cpp
  i++
  ```
  (It does not change anything for built-in types but it would bring consistency)


3. ##### Don't place any "using namespace" directives in headers.

4. ##### Compile time is without incidence. Increasing compile time to reduce execution time is encouraged.

5. ##### Code legibility and length is less important than easy and fast end-user experience.