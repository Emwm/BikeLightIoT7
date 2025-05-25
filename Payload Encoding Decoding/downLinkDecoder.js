function converter(code) {
  // Convert the input command object to JSON string
  const jsonString = (code);
  
  // Convert JSON string to hex string
  let hexString = '';
  for (let i = 0; i < jsonString.length; i++) {
    let hex = jsonString.charCodeAt(i).toString(16);
    if (hex.length < 2) hex = '0' + hex; // pad with zero if needed
    hexString += hex;
  }
  
  // Return hex string
  return hexString;
}