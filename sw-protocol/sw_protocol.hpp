#ifndef SW_JSON_HPP_
#define SW_JSON_HPP_
#include "rw_json.hpp"
#include "rw_xml.hpp"
namespace swjson {
    template <typename _type>
    static bool json_to_obj(const std::string& str, _type& t, bool isfile = false)
    {
        JsonReader reader(str, isfile);
        reader.convert(nullptr, t);
        return true;
    }
    //“indentCount = 0” 是展开json格式，"indentChar" 作为分割符
    template <typename _type>
    static std::string obj_to_json(const _type& data, const std::string& root = "", int indentCount = -1, char indentChar = ' ')
    {
        JsonWriter writer(indentCount, indentChar);
        writer.convert(root.c_str(), data);
        return writer.to_json_str();
    }
}//namespace swjson
namespace swxml {
    template <typename _type>
    static bool xml_to_obj(const std::string& str, _type& data, bool isfile = false)
    {
        XmlReader reader(str, isfile);
        reader.convert(nullptr, data);
        return true;
    }
    template <typename _type>
    static std::string obj_to_xml(const _type& data, const std::string& root = "")
    {
        XmlWriter writer;
        writer.convert(root.c_str(), data);
        return writer.to_xml_str();
    }
    template<typename _type>
    static bool obj_to_save_xml_file(const char* filename, const _type& data, const std::string& root = "")
    {
        XmlWriter writer;
        writer.convert(root.c_str(), data);
        return writer.save(filename);
    }
}//namespace swxml

//##NAME 前置连接 NAME## 后置连接
/******************************************Macro Meta Program******************************************/
#define STRUCT_FUNC_COMMON                                          \
public:                                                             \
    swjson::condition_t cond_t_;                                    \
/********************************************protocol_to_struct*******************************************/
#define STRUCT_FUNC_TOX_BEGIN(FNAME)                                \
  template<typename _doc>                                           \
    void FNAME##_to_struct(_doc& obj) {                              
// 绑定名称
#define STRUCT_ACT_TOX_O(M)                                         \
        obj.convert(#M, this->M);
//别名
#define STRUCT_ACT_TOX_A(M, A_NAME)                                 \
        obj.convert(swjson::alias_name_conversion(#M, A_NAME).c_str(), this->M);

#define STRUCT_FUNC_TOX_END }
/*******************************************struct_to_protocol*********************************************/
#define STRUCT_FUNC_TOS_BEGIN(FNAME)                                \
    template <typename _obj_type>                                   \
    void struct_to_##FNAME(_obj_type& obj, const char *root) const {
// 绑定名称
#define STRUCT_ACT_TOS_O(M)                                         \
        obj.convert(#M, this->M);
//别名
#define STRUCT_ACT_TOS_A(M, A_NAME)                                 \
        obj.convert(swjson::alias_name_conversion(#M, A_NAME).c_str(), this->M);

#define STRUCT_FUNC_TOS_END	}
/********************************************macro epand param*****************************************/
/*#define PARAM_SLOT_POS                                            \
_99,_98,_97,_96,_95,_94,_93,_92,_91,_90,                            \
_89,_88,_87,_86,_85,_84,_83,_82,_81,_80,                            \
_79,_78,_77,_76,_75,_74,_73,_72,_71,_70,                            \
_69,_68,_67,_66,_65,_64,_63,_62,_61,_60,                            \
_59,_58,_57,_56,_55,_54,_53,_52,_51,_50,                            \
_49,_48,_47,_46,_45,_44,_43,_42,_41,_40,                            \
_39,_38,_37,_36,_35,_34,_33,_32,_31,_30,                            \
_29,_28,_27,_26,_25,_24,_23,_22,_21,_20,                            \
_19,_18,_17,_16,_15,_14,_13,_12,_11,_10,                            \
_9,_8,_7,_6,_5,_4,_3,_2,_1 */

#define STRUCT_COUNT(LEVEL,ACTION,                                  \
_99,_98,_97,_96,_95,_94,_93,_92,_91,_90,                            \
_89,_88,_87,_86,_85,_84,_83,_82,_81,_80,                            \
_79,_78,_77,_76,_75,_74,_73,_72,_71,_70,                            \
_69,_68,_67,_66,_65,_64,_63,_62,_61,_60,                            \
_59,_58,_57,_56,_55,_54,_53,_52,_51,_50,                            \
_49,_48,_47,_46,_45,_44,_43,_42,_41,_40,                            \
_39,_38,_37,_36,_35,_34,_33,_32,_31,_30,                            \
_29,_28,_27,_26,_25,_24,_23,_22,_21,_20,                            \
_19,_18,_17,_16,_15,_14,_13,_12,_11,_10,                            \
_9,_8,_7,_6,_5,_4,_3,_2,_1, N,...) LEVEL##N

#define STRUCT_EXPAND(...) __VA_ARGS__

#define STRUCT_L1_TOX(x) STRUCT_L1_TOX_##x
#define STRUCT_L1_TOS(x) STRUCT_L1_TOS_##x
//string to obj
#define STRUCT_L1_TOX_O(...)  STRUCT_N2(STRUCT_L2, STRUCT_ACT_TOX_O, __VA_ARGS__)
#define STRUCT_L1_TOX_A(M,A)  STRUCT_ACT_TOX_A(M, A)
//obj to string
#define  STRUCT_L1_TOS_O(...) STRUCT_N2(STRUCT_L2, STRUCT_ACT_TOS_O, __VA_ARGS__)
#define  STRUCT_L1_TOS_A(M, A) STRUCT_ACT_TOS_A(M,A)

#ifdef _MSC_VER
#define STRUCT_N(LEVEL, ACTION, ...)                                \
STRUCT_EXPAND(STRUCT_COUNT(LEVEL, ACTION, __VA_ARGS__,              \
_99, _98, _97, _96, _95, _94, _93, _92, _91, _90,                   \
_89, _88, _87, _86, _85, _84, _83, _82, _81, _80,                   \
_79, _78, _77, _76, _75, _74, _73, _72, _71, _70,                   \
_69, _68, _67, _66, _65, _64, _63, _62, _61, _60,                   \
_59, _58, _57, _56, _55, _54, _53, _52, _51, _50,                   \
_49, _48, _47, _46, _45, _44, _43, _42, _41, _40,                   \
_39, _38, _37, _36, _35, _34, _33, _32, _31, _30,                   \
_29, _28, _27, _26, _25, _24, _23, _22, _21, _20,                   \
_19, _18, _17, _16, _15, _14, _13, _12, _11, _10,                   \
_9, _8, _7, _6, _5, _4, _3, _2, _1))                                \
STRUCT_EXPAND((ACTION, __VA_ARGS__))

#define STRUCT_N2(LEVEL, ACTION, ...)                               \
STRUCT_EXPAND(STRUCT_COUNT(LEVEL, ACTION, __VA_ARGS__,              \
_99, _98, _97, _96, _95, _94, _93, _92, _91, _90,                   \
_89, _88, _87, _86, _85, _84, _83, _82, _81, _80,                   \
_79, _78, _77, _76, _75, _74, _73, _72, _71, _70,                   \
_69, _68, _67, _66, _65, _64, _63, _62, _61, _60,                   \
_59, _58, _57, _56, _55, _54, _53, _52, _51, _50,                   \
_49, _48, _47, _46, _45, _44, _43, _42, _41, _40,                   \
_39, _38, _37, _36, _35, _34, _33, _32, _31, _30,                   \
_29, _28, _27, _26, _25, _24, _23, _22, _21, _20,                   \
_19, _18, _17, _16, _15, _14, _13, _12, _11, _10,                   \
_9, _8, _7, _6, _5, _4, _3, _2, _1))                                \
STRUCT_EXPAND((ACTION, __VA_ARGS__))
//question: https://stackoverflow.com/questions/5134523/msvc-doesnt-expand-va-args-correctly
#define STRUCT_L1_DEF(ACT, M)      ACT(M) // here will expand to ACT(O(xxx)), ACT(A(a,x)), ACT(M(xxx))
#define STRUCT_L1_1(ACT, M)        STRUCT_L1_DEF(ACT, M)
#define STRUCT_L1_2(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_1 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_3(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_2 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_4(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_3 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_5(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_4 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_6(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_5 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_7(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_6 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_8(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_7 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_9(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_8 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_10(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_9 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_11(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_10 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_12(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_11 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_13(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_12 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_14(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_13 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_15(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_14 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_16(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_15 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_17(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_16 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_18(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_17 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_19(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_18 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_20(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_19 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_21(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_20 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_22(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_21 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_23(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_22 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_24(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_23 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_25(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_24 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_26(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_25 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_27(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_26 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_28(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_27 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_29(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_28 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_30(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_29 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_31(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_30 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_32(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_31 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_33(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_32 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_34(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_33 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_35(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_34 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_36(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_35 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_37(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_36 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_38(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_37 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_39(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_38 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_40(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_39 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_41(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_40 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_42(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_41 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_43(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_42 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_44(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_43 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_45(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_44 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_46(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_45 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_47(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_46 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_48(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_47 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_49(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_48 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_50(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_49 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_51(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_50 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_52(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_51 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_53(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_52 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_54(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_53 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_55(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_54 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_56(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_55 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_57(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_56 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_58(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_57 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_59(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_58 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_60(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_59 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_61(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_60 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_62(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_61 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_63(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_62 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_64(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_63 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_65(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_64 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_66(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_65 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_67(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_66 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_68(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_67 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_69(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_68 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_70(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_69 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_71(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_70 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_72(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_71 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_73(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_72 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_74(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_73 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_75(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_74 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_76(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_75 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_77(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_76 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_78(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_77 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_79(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_78 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_80(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_79 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_81(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_80 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_82(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_81 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_83(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_82 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_84(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_83 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_85(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_84 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_86(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_85 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_87(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_86 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_88(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_87 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_89(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_88 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_90(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_89 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_91(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_90 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_92(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_91 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_93(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_92 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_94(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_93 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_95(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_94 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_96(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_95 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_97(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_96 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_98(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_97 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L1_99(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_98 STRUCT_EXPAND((ACT, __VA_ARGS__))

#define STRUCT_L2_DEF(ACT, M)     ACT(M)
#define STRUCT_L2_1(ACT, M)       STRUCT_L2_DEF(ACT, M)
#define STRUCT_L2_2(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_1 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_3(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_2 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_4(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_3 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_5(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_4 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_6(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_5 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_7(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_6 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_8(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_7 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_9(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_8 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_10(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_9 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_11(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_10 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_12(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_11 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_13(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_12 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_14(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_13 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_15(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_14 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_16(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_15 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_17(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_16 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_18(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_17 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_19(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_18 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_20(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_19 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_21(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_20 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_22(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_21 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_23(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_22 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_24(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_23 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_25(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_24 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_26(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_25 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_27(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_26 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_28(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_27 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_29(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_28 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_30(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_29 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_31(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_30 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_32(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_31 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_33(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_32 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_34(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_33 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_35(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_34 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_36(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_35 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_37(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_36 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_38(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_37 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_39(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_38 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_40(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_39 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_41(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_40 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_42(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_41 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_43(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_42 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_44(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_43 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_45(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_44 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_46(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_45 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_47(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_46 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_48(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_47 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_49(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_48 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_50(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_49 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_51(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_50 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_52(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_51 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_53(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_52 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_54(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_53 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_55(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_54 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_56(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_55 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_57(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_56 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_58(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_57 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_59(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_58 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_60(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_59 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_61(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_60 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_62(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_61 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_63(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_62 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_64(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_63 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_65(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_64 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_66(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_65 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_67(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_66 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_68(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_67 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_69(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_68 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_70(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_69 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_71(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_70 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_72(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_71 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_73(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_72 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_74(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_73 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_75(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_74 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_76(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_75 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_77(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_76 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_78(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_77 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_79(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_78 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_80(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_79 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_81(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_80 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_82(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_81 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_83(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_82 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_84(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_83 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_85(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_84 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_86(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_85 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_87(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_86 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_88(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_87 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_89(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_88 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_90(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_89 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_91(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_90 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_92(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_91 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_93(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_92 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_94(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_93 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_95(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_94 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_96(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_95 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_97(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_96 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_98(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_97 STRUCT_EXPAND((ACT, __VA_ARGS__))
#define STRUCT_L2_99(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_98 STRUCT_EXPAND((ACT, __VA_ARGS__))
#else
#define STRUCT_N(LEVEL, ACTION, ...)                                \
STRUCT_COUNT(LEVEL, ACTION, __VA_ARGS__,              				\
_99, _98, _97, _96, _95, _94, _93, _92, _91, _90,                   \
_89, _88, _87, _86, _85, _84, _83, _82, _81, _80,                   \
_79, _78, _77, _76, _75, _74, _73, _72, _71, _70,                   \
_69, _68, _67, _66, _65, _64, _63, _62, _61, _60,                   \
_59, _58, _57, _56, _55, _54, _53, _52, _51, _50,                   \
_49, _48, _47, _46, _45, _44, _43, _42, _41, _40,                   \
_39, _38, _37, _36, _35, _34, _33, _32, _31, _30,                   \
_29, _28, _27, _26, _25, _24, _23, _22, _21, _20,                   \
_19, _18, _17, _16, _15, _14, _13, _12, _11, _10,                   \
_9, _8, _7, _6, _5, _4, _3, _2, _1)                                 \
(ACTION, __VA_ARGS__)

#define STRUCT_N2(LEVEL, ACTION, ...)                               \
STRUCT_COUNT(LEVEL, ACTION, __VA_ARGS__,              				\
_99, _98, _97, _96, _95, _94, _93, _92, _91, _90,                   \
_89, _88, _87, _86, _85, _84, _83, _82, _81, _80,                   \
_79, _78, _77, _76, _75, _74, _73, _72, _71, _70,                   \
_69, _68, _67, _66, _65, _64, _63, _62, _61, _60,                   \
_59, _58, _57, _56, _55, _54, _53, _52, _51, _50,                   \
_49, _48, _47, _46, _45, _44, _43, _42, _41, _40,                   \
_39, _38, _37, _36, _35, _34, _33, _32, _31, _30,                   \
_29, _28, _27, _26, _25, _24, _23, _22, _21, _20,                   \
_19, _18, _17, _16, _15, _14, _13, _12, _11, _10,                   \
_9, _8, _7, _6, _5, _4, _3, _2, _1)                            		\
(ACTION, __VA_ARGS__)

#define STRUCT_L1_DEF(ACT, M)      ACT(M) // here will expand to ACT(O(xxx)), ACT(A(a,x)), ACT(M(xxx))
#define STRUCT_L1_1(ACT, M)        STRUCT_L1_DEF(ACT, M)
#define STRUCT_L1_2(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_1(ACT, __VA_ARGS__)
#define STRUCT_L1_3(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_2(ACT, __VA_ARGS__)
#define STRUCT_L1_4(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_3(ACT, __VA_ARGS__)
#define STRUCT_L1_5(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_4(ACT, __VA_ARGS__)
#define STRUCT_L1_6(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_5(ACT, __VA_ARGS__)
#define STRUCT_L1_7(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_6(ACT, __VA_ARGS__)
#define STRUCT_L1_8(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_7(ACT, __VA_ARGS__)
#define STRUCT_L1_9(ACT, M,...)    STRUCT_L1_DEF(ACT, M)      STRUCT_L1_8(ACT, __VA_ARGS__)
#define STRUCT_L1_10(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_9(ACT, __VA_ARGS__)
#define STRUCT_L1_11(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_10(ACT, __VA_ARGS__)
#define STRUCT_L1_12(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_11(ACT, __VA_ARGS__)
#define STRUCT_L1_13(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_12(ACT, __VA_ARGS__)
#define STRUCT_L1_14(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_13(ACT, __VA_ARGS__)
#define STRUCT_L1_15(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_14(ACT, __VA_ARGS__)
#define STRUCT_L1_16(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_15(ACT, __VA_ARGS__)
#define STRUCT_L1_17(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_16(ACT, __VA_ARGS__)
#define STRUCT_L1_18(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_17(ACT, __VA_ARGS__)
#define STRUCT_L1_19(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_18(ACT, __VA_ARGS__)
#define STRUCT_L1_20(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_19(ACT, __VA_ARGS__)
#define STRUCT_L1_21(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_20(ACT, __VA_ARGS__)
#define STRUCT_L1_22(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_21(ACT, __VA_ARGS__)
#define STRUCT_L1_23(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_22(ACT, __VA_ARGS__)
#define STRUCT_L1_24(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_23(ACT, __VA_ARGS__)
#define STRUCT_L1_25(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_24(ACT, __VA_ARGS__)
#define STRUCT_L1_26(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_25(ACT, __VA_ARGS__)
#define STRUCT_L1_27(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_26(ACT, __VA_ARGS__)
#define STRUCT_L1_28(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_27(ACT, __VA_ARGS__)
#define STRUCT_L1_29(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_28(ACT, __VA_ARGS__)
#define STRUCT_L1_30(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_29(ACT, __VA_ARGS__)
#define STRUCT_L1_31(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_30(ACT, __VA_ARGS__)
#define STRUCT_L1_32(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_31(ACT, __VA_ARGS__)
#define STRUCT_L1_33(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_32(ACT, __VA_ARGS__)
#define STRUCT_L1_34(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_33(ACT, __VA_ARGS__)
#define STRUCT_L1_35(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_34(ACT, __VA_ARGS__)
#define STRUCT_L1_36(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_35(ACT, __VA_ARGS__)
#define STRUCT_L1_37(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_36(ACT, __VA_ARGS__)
#define STRUCT_L1_38(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_37(ACT, __VA_ARGS__)
#define STRUCT_L1_39(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_38(ACT, __VA_ARGS__)
#define STRUCT_L1_40(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_39(ACT, __VA_ARGS__)
#define STRUCT_L1_41(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_40(ACT, __VA_ARGS__)
#define STRUCT_L1_42(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_41(ACT, __VA_ARGS__)
#define STRUCT_L1_43(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_42(ACT, __VA_ARGS__)
#define STRUCT_L1_44(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_43(ACT, __VA_ARGS__)
#define STRUCT_L1_45(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_44(ACT, __VA_ARGS__)
#define STRUCT_L1_46(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_45(ACT, __VA_ARGS__)
#define STRUCT_L1_47(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_46(ACT, __VA_ARGS__)
#define STRUCT_L1_48(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_47(ACT, __VA_ARGS__)
#define STRUCT_L1_49(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_48(ACT, __VA_ARGS__)
#define STRUCT_L1_50(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_49(ACT, __VA_ARGS__)
#define STRUCT_L1_51(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_50(ACT, __VA_ARGS__)
#define STRUCT_L1_52(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_51(ACT, __VA_ARGS__)
#define STRUCT_L1_53(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_52(ACT, __VA_ARGS__)
#define STRUCT_L1_54(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_53(ACT, __VA_ARGS__)
#define STRUCT_L1_55(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_54(ACT, __VA_ARGS__)
#define STRUCT_L1_56(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_55(ACT, __VA_ARGS__)
#define STRUCT_L1_57(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_56(ACT, __VA_ARGS__)
#define STRUCT_L1_58(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_57(ACT, __VA_ARGS__)
#define STRUCT_L1_59(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_58(ACT, __VA_ARGS__)
#define STRUCT_L1_60(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_59(ACT, __VA_ARGS__)
#define STRUCT_L1_61(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_60(ACT, __VA_ARGS__)
#define STRUCT_L1_62(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_61(ACT, __VA_ARGS__)
#define STRUCT_L1_63(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_62(ACT, __VA_ARGS__)
#define STRUCT_L1_64(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_63(ACT, __VA_ARGS__)
#define STRUCT_L1_65(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_64(ACT, __VA_ARGS__)
#define STRUCT_L1_66(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_65(ACT, __VA_ARGS__)
#define STRUCT_L1_67(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_66(ACT, __VA_ARGS__)
#define STRUCT_L1_68(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_67(ACT, __VA_ARGS__)
#define STRUCT_L1_69(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_68(ACT, __VA_ARGS__)
#define STRUCT_L1_70(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_69(ACT, __VA_ARGS__)
#define STRUCT_L1_71(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_70(ACT, __VA_ARGS__)
#define STRUCT_L1_72(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_71(ACT, __VA_ARGS__)
#define STRUCT_L1_73(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_72(ACT, __VA_ARGS__)
#define STRUCT_L1_74(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_73(ACT, __VA_ARGS__)
#define STRUCT_L1_75(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_74(ACT, __VA_ARGS__)
#define STRUCT_L1_76(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_75(ACT, __VA_ARGS__)
#define STRUCT_L1_77(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_76(ACT, __VA_ARGS__)
#define STRUCT_L1_78(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_77(ACT, __VA_ARGS__)
#define STRUCT_L1_79(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_78(ACT, __VA_ARGS__)
#define STRUCT_L1_80(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_79(ACT, __VA_ARGS__)
#define STRUCT_L1_81(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_80(ACT, __VA_ARGS__)
#define STRUCT_L1_82(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_81(ACT, __VA_ARGS__)
#define STRUCT_L1_83(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_82(ACT, __VA_ARGS__)
#define STRUCT_L1_84(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_83(ACT, __VA_ARGS__)
#define STRUCT_L1_85(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_84(ACT, __VA_ARGS__)
#define STRUCT_L1_86(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_85(ACT, __VA_ARGS__)
#define STRUCT_L1_87(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_86(ACT, __VA_ARGS__)
#define STRUCT_L1_88(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_87(ACT, __VA_ARGS__)
#define STRUCT_L1_89(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_88(ACT, __VA_ARGS__)
#define STRUCT_L1_90(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_89(ACT, __VA_ARGS__)
#define STRUCT_L1_91(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_90(ACT, __VA_ARGS__)
#define STRUCT_L1_92(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_91(ACT, __VA_ARGS__)
#define STRUCT_L1_93(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_92(ACT, __VA_ARGS__)
#define STRUCT_L1_94(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_93(ACT, __VA_ARGS__)
#define STRUCT_L1_95(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_94(ACT, __VA_ARGS__)
#define STRUCT_L1_96(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_95(ACT, __VA_ARGS__)
#define STRUCT_L1_97(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_96(ACT, __VA_ARGS__)
#define STRUCT_L1_98(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_97(ACT, __VA_ARGS__)
#define STRUCT_L1_99(ACT, M,...)   STRUCT_L1_DEF(ACT, M)      STRUCT_L1_98(ACT, __VA_ARGS__)

#define STRUCT_L2_DEF(ACT, M)     ACT(M)
#define STRUCT_L2_1(ACT, M)       STRUCT_L2_DEF(ACT, M)
#define STRUCT_L2_2(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_1(ACT, __VA_ARGS__)
#define STRUCT_L2_3(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_2(ACT, __VA_ARGS__)
#define STRUCT_L2_4(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_3(ACT, __VA_ARGS__)
#define STRUCT_L2_5(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_4(ACT, __VA_ARGS__)
#define STRUCT_L2_6(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_5(ACT, __VA_ARGS__)
#define STRUCT_L2_7(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_6(ACT, __VA_ARGS__)
#define STRUCT_L2_8(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_7(ACT, __VA_ARGS__)
#define STRUCT_L2_9(ACT, M,...)   STRUCT_L2_DEF(ACT, M)     STRUCT_L2_8(ACT, __VA_ARGS__)
#define STRUCT_L2_10(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_9(ACT, __VA_ARGS__)
#define STRUCT_L2_11(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_10(ACT, __VA_ARGS__)
#define STRUCT_L2_12(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_11(ACT, __VA_ARGS__)
#define STRUCT_L2_13(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_12(ACT, __VA_ARGS__)
#define STRUCT_L2_14(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_13(ACT, __VA_ARGS__)
#define STRUCT_L2_15(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_14(ACT, __VA_ARGS__)
#define STRUCT_L2_16(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_15(ACT, __VA_ARGS__)
#define STRUCT_L2_17(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_16(ACT, __VA_ARGS__)
#define STRUCT_L2_18(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_17(ACT, __VA_ARGS__)
#define STRUCT_L2_19(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_18(ACT, __VA_ARGS__)
#define STRUCT_L2_20(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_19(ACT, __VA_ARGS__)
#define STRUCT_L2_21(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_20(ACT, __VA_ARGS__)
#define STRUCT_L2_22(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_21(ACT, __VA_ARGS__)
#define STRUCT_L2_23(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_22(ACT, __VA_ARGS__)
#define STRUCT_L2_24(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_23(ACT, __VA_ARGS__)
#define STRUCT_L2_25(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_24(ACT, __VA_ARGS__)
#define STRUCT_L2_26(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_25(ACT, __VA_ARGS__)
#define STRUCT_L2_27(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_26(ACT, __VA_ARGS__)
#define STRUCT_L2_28(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_27(ACT, __VA_ARGS__)
#define STRUCT_L2_29(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_28(ACT, __VA_ARGS__)
#define STRUCT_L2_30(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_29(ACT, __VA_ARGS__)
#define STRUCT_L2_31(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_30(ACT, __VA_ARGS__)
#define STRUCT_L2_32(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_31(ACT, __VA_ARGS__)
#define STRUCT_L2_33(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_32(ACT, __VA_ARGS__)
#define STRUCT_L2_34(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_33(ACT, __VA_ARGS__)
#define STRUCT_L2_35(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_34(ACT, __VA_ARGS__)
#define STRUCT_L2_36(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_35(ACT, __VA_ARGS__)
#define STRUCT_L2_37(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_36(ACT, __VA_ARGS__)
#define STRUCT_L2_38(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_37(ACT, __VA_ARGS__)
#define STRUCT_L2_39(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_38(ACT, __VA_ARGS__)
#define STRUCT_L2_40(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_39(ACT, __VA_ARGS__)
#define STRUCT_L2_41(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_40(ACT, __VA_ARGS__)
#define STRUCT_L2_42(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_41(ACT, __VA_ARGS__)
#define STRUCT_L2_43(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_42(ACT, __VA_ARGS__)
#define STRUCT_L2_44(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_43(ACT, __VA_ARGS__)
#define STRUCT_L2_45(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_44(ACT, __VA_ARGS__)
#define STRUCT_L2_46(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_45(ACT, __VA_ARGS__)
#define STRUCT_L2_47(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_46(ACT, __VA_ARGS__)
#define STRUCT_L2_48(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_47(ACT, __VA_ARGS__)
#define STRUCT_L2_49(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_48(ACT, __VA_ARGS__)
#define STRUCT_L2_50(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_49(ACT, __VA_ARGS__)
#define STRUCT_L2_51(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_50(ACT, __VA_ARGS__)
#define STRUCT_L2_52(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_51(ACT, __VA_ARGS__)
#define STRUCT_L2_53(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_52(ACT, __VA_ARGS__)
#define STRUCT_L2_54(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_53(ACT, __VA_ARGS__)
#define STRUCT_L2_55(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_54(ACT, __VA_ARGS__)
#define STRUCT_L2_56(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_55(ACT, __VA_ARGS__)
#define STRUCT_L2_57(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_56(ACT, __VA_ARGS__)
#define STRUCT_L2_58(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_57(ACT, __VA_ARGS__)
#define STRUCT_L2_59(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_58(ACT, __VA_ARGS__)
#define STRUCT_L2_60(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_59(ACT, __VA_ARGS__)
#define STRUCT_L2_61(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_60(ACT, __VA_ARGS__)
#define STRUCT_L2_62(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_61(ACT, __VA_ARGS__)
#define STRUCT_L2_63(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_62(ACT, __VA_ARGS__)
#define STRUCT_L2_64(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_63(ACT, __VA_ARGS__)
#define STRUCT_L2_65(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_64(ACT, __VA_ARGS__)
#define STRUCT_L2_66(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_65(ACT, __VA_ARGS__)
#define STRUCT_L2_67(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_66(ACT, __VA_ARGS__)
#define STRUCT_L2_68(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_67(ACT, __VA_ARGS__)
#define STRUCT_L2_69(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_68(ACT, __VA_ARGS__)
#define STRUCT_L2_70(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_69(ACT, __VA_ARGS__)
#define STRUCT_L2_71(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_70(ACT, __VA_ARGS__)
#define STRUCT_L2_72(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_71(ACT, __VA_ARGS__)
#define STRUCT_L2_73(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_72(ACT, __VA_ARGS__)
#define STRUCT_L2_74(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_73(ACT, __VA_ARGS__)
#define STRUCT_L2_75(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_74(ACT, __VA_ARGS__)
#define STRUCT_L2_76(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_75(ACT, __VA_ARGS__)
#define STRUCT_L2_77(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_76(ACT, __VA_ARGS__)
#define STRUCT_L2_78(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_77(ACT, __VA_ARGS__)
#define STRUCT_L2_79(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_78(ACT, __VA_ARGS__)
#define STRUCT_L2_80(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_79(ACT, __VA_ARGS__)
#define STRUCT_L2_81(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_80(ACT, __VA_ARGS__)
#define STRUCT_L2_82(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_81(ACT, __VA_ARGS__)
#define STRUCT_L2_83(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_82(ACT, __VA_ARGS__)
#define STRUCT_L2_84(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_83(ACT, __VA_ARGS__)
#define STRUCT_L2_85(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_84(ACT, __VA_ARGS__)
#define STRUCT_L2_86(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_85(ACT, __VA_ARGS__)
#define STRUCT_L2_87(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_86(ACT, __VA_ARGS__)
#define STRUCT_L2_88(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_87(ACT, __VA_ARGS__)
#define STRUCT_L2_89(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_88(ACT, __VA_ARGS__)
#define STRUCT_L2_90(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_89(ACT, __VA_ARGS__)
#define STRUCT_L2_91(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_90(ACT, __VA_ARGS__)
#define STRUCT_L2_92(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_91(ACT, __VA_ARGS__)
#define STRUCT_L2_93(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_92(ACT, __VA_ARGS__)
#define STRUCT_L2_94(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_93(ACT, __VA_ARGS__)
#define STRUCT_L2_95(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_94(ACT, __VA_ARGS__)
#define STRUCT_L2_96(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_95(ACT, __VA_ARGS__)
#define STRUCT_L2_97(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_96(ACT, __VA_ARGS__)
#define STRUCT_L2_98(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_97(ACT, __VA_ARGS__)
#define STRUCT_L2_99(ACT, M,...)  STRUCT_L2_DEF(ACT, M)     STRUCT_L2_98(ACT, __VA_ARGS__)
#endif
/*********************************************************************************************************/
#define SW_JSON(...)\
    STRUCT_FUNC_COMMON\
    STRUCT_FUNC_TOX_BEGIN(json) STRUCT_N(STRUCT_L1, STRUCT_L1_TOX, __VA_ARGS__) STRUCT_FUNC_TOX_END\
    STRUCT_FUNC_TOS_BEGIN(json) STRUCT_N(STRUCT_L1, STRUCT_L1_TOS, __VA_ARGS__) STRUCT_FUNC_TOS_END

#define SW_XML(...)\
    STRUCT_FUNC_COMMON\
    STRUCT_FUNC_TOX_BEGIN(xml) STRUCT_N(STRUCT_L1, STRUCT_L1_TOX, __VA_ARGS__) STRUCT_FUNC_TOX_END\
    STRUCT_FUNC_TOS_BEGIN(xml) STRUCT_N(STRUCT_L1, STRUCT_L1_TOS, __VA_ARGS__) STRUCT_FUNC_TOS_END

#define SW_DB(...)\
    STRUCT_FUNC_COMMON\
    STRUCT_FUNC_TOX_BEGIN(db) STRUCT_N(STRUCT_L1, STRUCT_L1_TOX, __VA_ARGS__) STRUCT_FUNC_TOX_END\
    STRUCT_FUNC_TOS_BEGIN(db) STRUCT_N(STRUCT_L1, STRUCT_L1_TOS, __VA_ARGS__) STRUCT_FUNC_TOS_END

#define SW_PROTOCOL(...) SW_XML(__VA_ARGS__)
#endif