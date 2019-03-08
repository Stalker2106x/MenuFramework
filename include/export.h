#ifndef EXPORT_H_
#define EXPORT_H_

#ifdef _MSC_VER
  #define MF_API __declspec(dllexport)
#else
  #define MF_API
#endif

#endif /* !EXPORT_H_ */