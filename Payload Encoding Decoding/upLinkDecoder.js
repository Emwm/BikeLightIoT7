function converter(code) {
  try {
    // Convert hex string to plain text
    function hexToString(hex) {
      let str = '';
      for (let i = 0; i < hex.length; i += 2) {
        const charCode = parseInt(hex.substr(i, 2), 16);
        str += String.fromCharCode(charCode);
      }
      return str;
    }
    
    // Ensure the code is a string
    if (typeof code !== 'string') {
      code = String(code);
    }
    
    // Convert hex to plain text
    const textPayload = hexToString(code);
    const values = textPayload.split(',');
    const result = {};
    
    // devMode
    if (values.length >= 1 && values[0].trim() !== '') {
      result.devMode = { value: parseFloat(values[0].trim()) };
    }
    
    // batteryLevel
    if (values.length >= 2 && values[1].trim() !== '') {
      result.batteryLevel = { value: parseFloat(values[1].trim()) };
    }
    
    // Location: expects values[2] (lat), values[3] (lon)
    if (
      values.length >= 4 && 
      values[2].trim() !== '' && 
      values[3].trim() !== '' &&
      !isNaN(parseFloat(values[2])) &&
      !isNaN(parseFloat(values[3]))
    ) {
      result.location = { 
        value: {
          latitude: parseFloat(values[2].trim()),
          longitude: parseFloat(values[3].trim())
        }
      };
    }
    
    return JSON.stringify(result);
  } catch (error) {
    console.error("Error decoding payload:", error);
    return JSON.stringify({ error: error.message });
  }
}