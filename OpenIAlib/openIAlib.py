from openai import OpenAI

client = OpenAI(api_key="sk-6730797685e8475392411913b3a9bb04", base_url="https://api.deepseek.com")

def call_deepseek_reasoner(prompt):
    try:
        response = client.chat.completions.create(
            model="deepseek-reasoner",  # Verify the correct model name
            messages=[
                {"role": "system", "content": "You are a helpful AI assistant."},
                {"role": "user", "content": prompt}
            ],
            temperature=0.7,  # Adjust for creativity (0-1)
        )
        return response.choices[0].message.content
    except Exception as e:
        return f"Error calling API: {e}"

# Example usage
if __name__ == "__main__":
    user_prompt = "Explain quantum computing in simple terms."
    answer = call_deepseek_reasoner(user_prompt)
    print("DeepSeek Reasoner's Response:")
    print(answer)
