module.exports = [
  {
    "type": "heading",
    "defaultValue": "Watchface Configuration"
  },
  {
    "type": "text",
    "defaultValue": "Customize the way your watchface looks!"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Main Colors"
      },
      {
        "type": "color",
        "messageKey": "BackgroundColor",
        "defaultValue": "0x0000AA",
        "allowGray": "true",
        "label": "Background Color"
      },
      {
        "type": "color",
        "messageKey": "TextColor",
        "defaultValue": "0xFFFFFF",
        "label": "Foreground Color"
      },
      {
        "type": "radiogroup",
        "messageKey": "ImageColor",
        "defaultValue": "w\0",
        "label": "Trinity Color",
        "options": [
          { 
            "label": "White", 
            "value": "w\0" 
          },
          { 
            "label": "Black", 
            "value": "b\0" 
          }
        ]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Battery Bar Colors"
      },
      {
        "type": "color",
        "messageKey": "BattFGColor",
        "defaultValue": "0xFFFFFF",
        "allowGray": "true",
        "label": "Battery Bar Foreground"
      },
      {
        "type": "color",
        "messageKey": "BattBGColor",
        "defaultValue": "0x000000",
        "allowGray": "true",
        "label": "Battery Bar Background"
      },
      {
        "type": "color",
        "messageKey": "P20BattColor",
        "defaultValue": "0xFF5500",
        "allowGray": "true",
        "label": "Battery Warning Color (Below 20%)"
      },
      {
        "type": "color",
        "messageKey": "P10BattColor",
        "defaultValue": "0xFF0000",
        "allowGray": "true",
        "label": "Battery Warning Color (Below 10%)"
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
