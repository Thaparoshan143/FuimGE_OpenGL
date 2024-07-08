#pragma once

#include"./Types.h"
#include"./TSingleton.hpp"
#include <map>

namespace Input
{
    // Here the 8 bit are arrange in such a way that the mask will enable it to get the appropriate result in & bit mask 
    // Bit arrangement : 00000111 <- keyboard
    //                 : 11111000 <- mouse
    typedef enum EventType : uchar
    {
        KEYBOARD = 7,
        KEYBOARD_PRESS = 2,
        KEYBOARD_RELEASE = 1,

        MOUSE = 248,
        MOUSE_BTN = 24,
        MOUSE_PRESS = 16,
        MOUSE_RELEASE = 8,
        MOUSE_MOVE = 32,
        MOUSE_DRAG = 64
    }EventType;
}

typedef void (*DispatchCallback)(void *event);

template<class T>
class TEventQueue
{
    public:
    TEventQueue();
    void CallbackDispatch(DispatchCallback eventCallback)
    {
        for(int i=0;i<m_eventQueue.size();i++)
        {
            eventCallback(m_eventQueue[i]);
        }
        m_eventQueue.clear();
    }
    void AddToQueue(T event)
    {
        m_eventQueue.push_back(event);
    }

    std::vector<T> m_eventQueue;
};

namespace Interface
{
    class IEvent
    {
        public:
        inline bool IsEnabled() {   return m_isEnabled;  }
        inline void SetStatus(bool enable)  {   m_isEnabled = enable;    }
        inline virtual bool IsEvent(int mask) = 0;

        bool m_isEnabled;
    };

    class IKeyEvent : public IEvent
    {
        public:
        virtual bool IsPressed() = 0;
        virtual bool IsReleased() = 0;
        virtual bool IsRepeat() = 0;
        inline virtual int GetKey()   {   return m_keyCode;   }
        inline virtual int GetStatus()    {   return m_status;    }
        inline virtual bool IsKey(int keyMask)    {   return m_keyCode == keyMask;  }
        inline virtual bool IsStatus(int statusMask)  {   return m_status == statusMask;  }

        inline virtual bool IsEvent(int mask) override  {   return m_status & mask; }

        int m_keyCode;
        int m_status;
    };

    class IMouseEvent : public IEvent
    {
        public:
        virtual bool IsPressed() = 0;
        virtual bool IsReleased() = 0;
        virtual bool IsRepeat() = 0;
        inline virtual int GetButton()   {   return m_button;   }
        inline virtual int GetStatus()    {   return m_status;    }
        inline virtual bool IsButton(int btnMask)    {   return m_button == btnMask;  }
        inline virtual bool IsStatus(int statusMask)  {   return m_status == statusMask;  }

        inline virtual bool IsEvent(int mask) override  {   return m_status & mask; }

        int m_button;
        int m_status;
    };


    class IMouseMoveEvent : public IEvent
    {
        public:
        inline virtual bool IsMoving() {   return m_isMoving; }
        inline virtual bool IsDeltaGreater(fVec2 compDelta)    {   return (m_deltaMov.x >= compDelta.x || m_deltaMov.y >= compDelta.y); }
        inline virtual fVec2 GetDeltaMov() {   return m_deltaMov;  }
        inline virtual fVec2 GetLastPos()  {   return m_lastPosition;  }
        inline virtual fVec2 GetCurrentPosition()  {   return m_currentPosition;   }

        inline virtual bool IsEvent(int mask) override  {   return mask == Input::EventType::MOUSE_MOVE; }

        fVec2 m_currentPosition;
        fVec2 m_lastPosition;
        fVec2 m_deltaMov;
        bool m_isMoving;
    };

    class IMouseDragEvent : public IMouseMoveEvent
    {
        public:
        inline virtual bool IsDraging()    {   return m_isBtnClicked & IsMoving();    }
        inline virtual bool IsDrag(int btnMask)   {   return (m_button == btnMask && IsMoving()); }
        inline virtual fVec2 GetDragDeltaMov() {   return GetDeltaMov();   }
        
        inline virtual bool IsEvent(int mask) override  {   return mask == Input::EventType::MOUSE_DRAG; }

        int m_button;
        bool m_isBtnClicked;
    };


    class IEventHandler : public Singleton::TISingleton<IEventHandler>
    {
        public:
        IEventHandler() : TISingleton(this) {   }

        void RegisterCallbacks(std::map<Input::EventType, DispatchCallback> callbackMap)
        {
            m_callbacks = callbackMap;
        }
        const std::map<Input::EventType, DispatchCallback> GetCallbackMap() {   return m_callbacks; }
        template<typename T>
        void AddEvent(Input::EventType type, T event)
        {
            switch(type)
            {
                case Input::EventType::KEYBOARD:
                    m_keyEvent.push_back(event);
                break;
                case Input::EventType::MOUSE_BTN:
                    m_mouseBtnEvent.push_back(event);
                break;
                case Input::EventType::MOUSE_MOVE:
                    m_mouseMovEvent.push_back(event);
                break;
                case Input::EventType::MOUSE_DRAG:
                    m_mouseDragEvent.push_back(event);
                break;
                default:
                    std::cout << "Unknown Event register! Unable to handle" << std::endl;
                break;
            }
        }

        void DispatchByType(Input::EventType type)
        {
            if(m_callbacks.count(type) == 0)
            {
                std::cout << "Callback Not registered" << std::endl;
            }
            else
            {
                switch(type)
                {
                    case Input::EventType::KEYBOARD:
                        iterateVectorCallback(m_keyEvent, m_callbacks[type]);
                    break;
                    case Input::EventType::MOUSE_BTN:
                        iterateVectorCallback(m_mouseBtnEvent, m_callbacks[type]);
                    break;
                    case Input::EventType::MOUSE_MOVE:
                        iterateVectorCallback(m_mouseMovEvent, m_callbacks[type]);
                    break;
                    case Input::EventType::MOUSE_DRAG:
                        iterateVectorCallback(m_mouseDragEvent, m_callbacks[type]);
                    break;
                    default:
                        std::cout << "Event Unknow type" << std::endl;
                    break;
                }
            }
        }

        void DispatchAll()
        {
            iterateVectorCallback(m_keyEvent, m_callbacks[Input::EventType::KEYBOARD]);
            iterateVectorCallback(m_mouseBtnEvent, m_callbacks[Input::EventType::MOUSE_BTN]);
            iterateVectorCallback(m_mouseMovEvent, m_callbacks[Input::EventType::MOUSE_MOVE]);
            iterateVectorCallback(m_mouseDragEvent, m_callbacks[Input::EventType::MOUSE_DRAG]);
        }

        std::map<Input::EventType, DispatchCallback> m_callbacks;
        // using vector for now and not using below
        // TEventQueue<IMouseEvent> m_mouseBtnEvent;
        // TEventQueue<IKeyEvent> m_keyEvent;
        // TEventQueue<IMouseMoveEvent> m_mouseMovEvent;
        // TEventQueue<IMouseDragEvent> m_mouseDragEvent;
        std::vector<IMouseEvent> m_mouseBtnEvent;
        std::vector<IKeyEvent> m_keyEvent;
        std::vector<IMouseMoveEvent> m_mouseMovEvent;
        std::vector<IMouseDragEvent> m_mouseDragEvent;

        protected:
        template<typename T>
        void iterateVectorCallback(std::vector<T> &vector, DispatchCallback callback)
        {
            if(callback == nullptr)
            {
                std::cout << "callback passed is null ! can't iterate over it" << std::endl;
                return;
            }
            for(int i=0;i<vector.size();++i)
            {
                callback(vector[i]);
            }
        }
    };
}