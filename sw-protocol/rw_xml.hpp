#ifndef RW_XML_HPP_
#define RW_XML_HPP_
#include <vector>
#include <list>
#include <memory>
#include <stack>
#include <tinyxml/tinyxml2.h>
#include "rw_traits_type.hpp"


namespace swxml
{
using namespace tinyxml2;
using namespace swtraits;
class RWriter
{
protected:
    std::stack<XMLNode*> stack_node_;
    XMLNode* root_;
    
    const char* key_;

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
class XmlReader: public RWriter
{
private:
    std::unique_ptr<XMLDocument> doc_;
public:
    XmlReader(const std::string& str, bool isfile = false) 
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
#define XML_GETVAL(func, ...)                                \
        if(nullptr == key){                                  \
            printf("key value empty.\n");                    \
            return false;                                    \
        }                                                    \
        XMLNode* node = get_current_node();                  \
        if(nullptr == node){                                 \
            printf("key:%s\n, node exist.\n", key);          \
            return true;                                     \
        }                                                    \
        val = __VA_ARGS__ node->ToElement()->func(key);      \
        return true;

    bool convert(const char* key, std::string& val)
    {
        //XMLNode* node = get_current_node();
        //printf("%s\n", node->Value());
        //node->ToElement()->Attribute(key);
        //return true;
        XML_GETVAL(Attribute);
    }
    bool convert(const char* key, int8_t& val)
    {
        XML_GETVAL(IntAttribute, (int8_t));
    }
    bool convert(const char* key, uint8_t& val)
    {
        XML_GETVAL(IntAttribute, (uint8_t));
    }
    bool convert(const char* key, int16_t& val)
    {
        XML_GETVAL(IntAttribute, (int16_t));
    }
    bool convert(const char* key, uint16_t& val)
    {
        XML_GETVAL(IntAttribute, (uint16_t));
    }
    bool convert(const char* key, int32_t& val)
    {
        XML_GETVAL(IntAttribute);
    }
    bool convert(const char* key, uint32_t& val)
    {
        XML_GETVAL(UnsignedAttribute);
    }
    bool convert(const char* key, int64_t& val)
    {
        XML_GETVAL(Int64Attribute);
    }
    bool convert(const char* key, uint64_t& val)
    {
        XML_GETVAL(Unsigned64Attribute);
    }
    bool convert(const char* key, double& val)
    {
        XML_GETVAL(DoubleAttribute);
    }
    bool convert(const char* key, float& val)
    {
        XML_GETVAL(FloatAttribute);
    }
    bool convert(const char* key, bool& val)
    {
        XML_GETVAL(BoolAttribute);
    }
    template<typename _type, typename = typename std::enable_if<std::is_class<_type>::value>::type>
    bool convert(const char* key, std::vector<_type>& val)
    {
        XMLNode* node = get_node(key);
        if (nullptr == node)
            return false;
        auto num = this->size();
        val.resize(num);
        for (auto i = 0; i < num ; ++i)
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
    template<typename _type, typename std::enable_if<has_member_condition_t<_type>::value, int>::type = 0>
    bool convert(const char* key, _type& val)
    {
        begin_object(key);
        val.xml_to_struct(*this);
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
    XmlWriter()
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
    bool save(const char* filename)
    {
        if (nullptr == filename)
            return false;

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
        node->ToElement()->SetAttribute(key, val.c_str());
        return *this;
    }
    XmlWriter& convert(const char* key, int16_t val)
    {
        XMLNode* node = get_current_node();
        node->ToElement()->SetAttribute(key, val);
        return *this;
    }
    XmlWriter& convert(const char* key, uint16_t val)
    {
        XMLNode* node = get_current_node();
        node->ToElement()->SetAttribute(key, val);
        return *this;
    }
    XmlWriter& convert(const char* key, int32_t val)
    {
        XMLNode* node = get_current_node();
        node->ToElement()->SetAttribute(key, val);
        return *this;
    }
    XmlWriter& convert(const char* key, uint32_t val)
    {
        XMLNode* node = get_current_node();
        node->ToElement()->SetAttribute(key, val);
        return *this;
    }
    XmlWriter& convert(const char* key, int64_t val)
    {
        XMLNode* node = get_current_node();
        node->ToElement()->SetAttribute(key, val);
        return *this;
    }
    XmlWriter& convert(const char* key, uint64_t val)
    {
        XMLNode* node = get_current_node();
        node->ToElement()->SetAttribute(key, val);
        return *this;
    }
    XmlWriter& convert(const char* key, double val)
    {
        XMLNode* node = get_current_node();
        node->ToElement()->SetAttribute(key, val);
        return *this;
    }
    XmlWriter& convert(const char* key, float val)
    {
        XMLNode* node = get_current_node();
        node->ToElement()->SetAttribute(key, val);
        return *this;
    }
    XmlWriter& convert(const char* key, bool val)
    {
        XMLNode* node = get_current_node();
        node->ToElement()->SetAttribute(key, val);
        return *this;
    }
    template<typename _type, typename = typename std::enable_if<std::is_class<_type>::value>::type>
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
    template <typename _type>
    void convert(const char* key, const _type& data)
    {
        this->begin_object(key);
        data.struct_to_xml(*this, key);
        this->end_object();
    }
};
} //namespace swxml
#endif //RW_XML_HPP_
