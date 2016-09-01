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
        "defaultValue": "Colors"
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
        "defaultValue": "w",
        "label": "Trinity Color",
        "options": [
          { 
            "label": "White", 
            "value": "w" 
          },
          { 
            "label": "Black", 
            "value": "b" 
          }
        ]
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
