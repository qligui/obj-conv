#ifndef RW_XML_HPP_
#define RW_XML_HPP_
#include <vector>
#include <list>
#include <memory>
#include <stack>
#include <typeinfo>
#include <tinyxml/tinyxml2.h>
#include "rw_traits_type.hpp"

namespace reflexxml
{
using namespace tinyxml2;
using namespace reflextraits;
class RWriter
{
public:
    RWriter() { }
    RWriter(bool attribute = false) :attribute_(attribute) { }
    ~RWriter() { }
protected:
    std::stack<XMLNode*> stack_node_;
    XMLNode* root_;
    const char* key_;
    bool attribute_;

protected:
    XMLNode* get_current_node()
    {
        return stack_node_.empty() ? nullptr : stack_node_.top();
    }
    void push_root(XMLNode* root)
    {
        root_ = root;
        stack_node_.push(root);
    }
    void pop_root()
    {
        if (stack_node_.top() != root_ && stack_node_.size() == 1)
        {
            printf("root error!\n");
            return;
        }
        root_ = nullptr;
        stack_node_.pop();
    }
    XMLNode* get_node(const char* key)
    {
        XMLNode* node = stack_node_.empty() ? nullptr : get_current_node();
        if (nullptr != key)
        {
            key_ = key;
        }
        return node->FirstChildElement(key);
    }
};
class XmlReader : public RWriter
{
private:
    std::unique_ptr<XMLDocument> doc_;
public:
    XmlReader(const std::string& str, bool attribute, bool isfile = false):RWriter(attribute)
    {
        doc_ = std::make_unique<XMLDocument>();
        isfile ? doc_->LoadFile(str.c_str()) : doc_->Parse(str.c_str());
        XMLNode* root = doc_->NewElement("");
        push_root(root);
    }
    ~XmlReader()
    {
        pop_root();
    }

    void begin_object(const char* key)
    {
        XMLNode* node = get_current_node();
        XMLNode* chlid_node = key == nullptr ? node->LinkEndChild(doc_->RootElement()) : node->FirstChildElement(key);
        stack_node_.push(chlid_node);
    }
    void end_object()
    {
        if (stack_node_.empty())
            return;
        stack_node_.pop();
    }
#define XML_GETVAL(attfunc, textfunc, ...)                      \
        if(nullptr == key){                                     \
           printf("key value empty.\n");                        \
           return false;                                        \
        }                                                       \
        XMLNode* node = get_current_node();                     \
        if(nullptr == node){                                    \
            printf("key attibute :%s,node exist\n", key);       \
            return true;                                        \
        }                                                       \
        if(attribute_){                                         \
          val = __VA_ARGS__(node->ToElement()->attfunc(key));   \
        }                                                       \
        else {                                                  \
          XMLElement* child_elem = node->FirstChildElement(key);\
           if(nullptr == child_elem) {                          \
              printf("key label:%s, node exist.\n", key);       \
              return true;                                      \
           }                                                    \
           if(typeid(val) == typeid(std::string)){              \
              if(nullptr == child_elem->GetText()){             \
                  printf("key label value is null.\n");         \
                  return true;                                  \
              }                                                 \
           }                                                    \
           val = __VA_ARGS__(child_elem->textfunc());           \
        }                                                       \
        return true;

    bool convert(const char* key, std::string& val)
    {
        XML_GETVAL(Attribute, GetText, (std::string));
    }
    bool convert(const char* key, int8_t& val)
    {
        XML_GETVAL(IntAttribute, IntText, (int8_t));
    }
    bool convert(const char* key, uint8_t& val)
    {
        XML_GETVAL(IntAttribute, IntText, (uint8_t));
    }
    bool convert(const char* key, int16_t& val)
    {
        XML_GETVAL(IntAttribute, IntText,(int16_t));
    }
    bool convert(const char* key, uint16_t& val)
    {
        XML_GETVAL(IntAttribute, IntText, (uint16_t));
    }
    bool convert(const char* key, int32_t& val)
    {
        XML_GETVAL(IntAttribute, IntText);
    }
    bool convert(const char* key, uint32_t& val)
    {
        XML_GETVAL(UnsignedAttribute, UnsignedText);
    }
    bool convert(const char* key, int64_t& val)
    {
        XML_GETVAL(Int64Attribute, Int64Text);
    }
    bool convert(const char* key, uint64_t& val)
    {
        XML_GETVAL(Unsigned64Attribute, Unsigned64Text);
    }
    bool convert(const char* key, double& val)
    {
        XML_GETVAL(DoubleAttribute, DoubleText);
    }
    bool convert(const char* key, float& val)
    {
        XML_GETVAL(FloatAttribute, FloatText);
    }
    bool convert(const char* key, bool& val)
    {
        XML_GETVAL(BoolAttribute, BoolText);
    }
    template<typename _type, typename = std::enable_if_t<std::is_class_v<_type>>>
    bool convert(const char* key, std::vector<_type>& val)
    {
        XMLNode* node = get_node(key);
        if (nullptr == node)
            return false;
        auto num = this->size();
        val.resize(num);
        for (auto i = 0; i < num; ++i)
        {
            this->convert(key, val[i]);
        }
        return true;
    }
    template<typename _type>
    bool convert(const char* key, std::list<_type>& val)
    {
        XMLNode* node = get_node(key);
        if (nullptr == node)
            return false;
        auto num = this->size();
        for (int i = 0; i < num; ++i)
        {
            _type elem;
            this->convert(key, elem);
            val.emplace_back(elem);
        }
        return true;
    }
    template<typename _type>
    bool convert(const char* key, std::set<_type>& val)
    {
        XMLNode* node = get_node(key);
        if (nullptr == node)
            return false;
        auto num = this->size();
        for (int i = 0; i < num; ++i)
        {
            _type elem;
            this->convert(key, elem);
            val.insert(elem);
        }
        return true;
    }
    template<typename _type>
    bool convert(const char* key, std::shared_ptr<_type>& val)
    {
        if (nullptr == val.get())
        {
            val.reset(new _type());
        }
        return this->convert(key, *val);
    }
    template<typename _type, typename = std::enable_if_t<std::is_enum_v<_type>>>
    bool convert(const char* key, _type& enum_val)
    {
        typename std::underlying_type<_type>::type val;
        this->convert(key, val);
        enum_val = static_cast<_type>(val);
        return true;
    }
    template<typename _type, typename std::enable_if_t<has_member_condition<_type>::value, bool> = true>
    bool convert(const char* key, _type& val)
    {
        begin_object(key);
        val.obj_to_struct(*this);
        end_object();
        return true;
    }
    size_t size()
    {
        size_t counts = 0;
        XMLNode* node = get_node(key_);
        for (XMLNode* pnode = node; pnode != nullptr; pnode = pnode->NextSiblingElement(key_), ++counts);
        return counts;
    }
};

class XmlWriter : public RWriter
{
private:
    std::unique_ptr<XMLDocument> doc_;

public:
    XmlWriter(bool  attribute = false) :RWriter(attribute)
    {
        doc_ = std::make_unique<XMLDocument>();
        const char* declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
        doc_->Parse(declaration);
        push_root(doc_->NewElement(""));
    }
    ~XmlWriter()
    {
        pop_root();
    }

    std::string to_xml_str()
    {
        XMLPrinter printer;
        doc_->LinkEndChild(root_->FirstChild());
        doc_->Print(&printer);
        return std::string(printer.CStr());
    }
    bool save_xml_file(const char* filename)
    {
        if (nullptr == filename)
            return false;
        doc_->LinkEndChild(root_->FirstChild());
        doc_->SaveFile(filename);
        return true;
    }

    void begin_object(const char* key)
    {
        XMLNode* node = doc_->NewElement(key);
        stack_node_.push(get_current_node()->InsertEndChild(node));
    }
    void end_object()
    {
        if (stack_node_.empty())
            return;
        stack_node_.pop();
    }
    XmlWriter& convert(const char* key, const std::string& val)
    {
        XMLNode* node = get_current_node();
        if (attribute_)
        {
            node->ToElement()->SetAttribute(key, val.c_str());
        }
        else
        {
            XMLElement* element = doc_->NewElement(key);
            element->SetText(val.c_str());
            node->InsertEndChild(element);
        }
        return *this;
    }
    XmlWriter& convert(const char* key, int16_t val)
    {
        XMLNode* node = get_current_node();
        if (attribute_)
        {
            node->ToElement()->SetAttribute(key, val);
        }
        else
        {
            XMLElement* element = doc_->NewElement(key);
            element->SetText(val);
            node->InsertEndChild(element);
        }
        return *this;
    }
    XmlWriter& convert(const char* key, uint16_t val)
    {
        XMLNode* node = get_current_node();
        if (attribute_)
        {
            node->ToElement()->SetAttribute(key, val);
        }
        else
        {
            XMLElement* element = doc_->NewElement(key);
            element->SetText(val);
            node->InsertEndChild(element);
        }
        return *this;
    }
    XmlWriter& convert(const char* key, int32_t val)
    {
        XMLNode* node = get_current_node();
        if (attribute_)
        {
            node->ToElement()->SetAttribute(key, val);
        }
        else
        {
            XMLElement* element = doc_->NewElement(key);
            element->SetText(val);
            node->InsertEndChild(element);
        }
        return *this;
    }
    XmlWriter& convert(const char* key, uint32_t val)
    {
        XMLNode* node = get_current_node();
        if (attribute_)
        {
            node->ToElement()->SetAttribute(key, val);
        }
        else
        {
            XMLElement* element = doc_->NewElement(key);
            element->SetText(val);
            node->InsertEndChild(element);
        }
        return *this;
    }
    XmlWriter& convert(const char* key, int64_t val)
    {
        XMLNode* node = get_current_node();
        if (attribute_)
        {
            node->ToElement()->SetAttribute(key, val);
        }
        else
        {
            XMLElement* element = doc_->NewElement(key);
            element->SetText(val);
            node->InsertEndChild(element);
        }
        return *this;
    }
    XmlWriter& convert(const char* key, uint64_t val)
    {
        XMLNode* node = get_current_node();
        if (attribute_)
        {
            node->ToElement()->SetAttribute(key, val);
        }
        else
        {
            XMLElement* element = doc_->NewElement(key);
            element->SetText(val);
            node->InsertEndChild(element);
        }
        return *this;
    }
    XmlWriter& convert(const char* key, double val)
    {
        XMLNode* node = get_current_node();
        if (attribute_)
        {
            node->ToElement()->SetAttribute(key, val);
        }
        else
        {
            XMLElement* element = doc_->NewElement(key);
            element->SetText(val);
            node->InsertEndChild(element);
        }
        return *this;
    }
    XmlWriter& convert(const char* key, float val)
    {
        XMLNode* node = get_current_node();
        if (attribute_)
        {
            node->ToElement()->SetAttribute(key, val);
        }
        else
        {
            XMLElement* element = doc_->NewElement(key);
            element->SetText(val);
            node->InsertEndChild(element);
        }
        return *this;
    }
    XmlWriter& convert(const char* key, bool val)
    {
        XMLNode* node = get_current_node();
        if (attribute_)
        {
            node->ToElement()->SetAttribute(key, val);
        }
        else
        {
            XMLElement* element = doc_->NewElement(key);
            element->SetText(val);
            node->InsertEndChild(element);
        }
        return *this;
    }
	template<typename _type, typename = std::enable_if_t<std::is_enum_v<_type>>>
	void convert(const char* key, const _type& val)
	{
		convert(key, static_cast<typename std::underlying_type<_type>::type>(val));
	}
    template<typename _type, typename = std::enable_if_t<std::is_class_v<_type>>>
    XmlWriter& convert(const char* key, const std::vector<_type>& data)
    {
        for (auto i = 0; i < data.size(); ++i)
        {
            this->convert(key, data[i]);
        }
        return *this;
    }
    template<typename _type>
    XmlWriter& convert(const char* key, const std::list<_type>& data)
    {
        for (typename std::list<_type>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
        {
            this->convert(key, *iter);
        }
        return *this;
    }
    template<typename _type>
    XmlWriter& convert(const char* key, const std::set<_type>& data)
    {
        for (typename std::set<_type>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
        {
            this->convert(key, *iter);
        }
        return *this;
    }
    template <typename _type>
    void convert(const char* key, const std::shared_ptr<_type>& val)
    {
        if (nullptr == val.get())
            return;

        this->convert(key, *val);
    }
    template <typename _type, typename std::enable_if_t<has_member_condition<_type>::value, bool> = true>
    void convert(const char* key, const _type& data)
    {
        this->begin_object(key);
        data.struct_to_obj(*this, key);
        this->end_object();
    }
};
} //namespace reflexxml
#endif //RW_XML_HPP_
