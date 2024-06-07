import { createRoot } from "react-dom/client";
import { ClerkProvider, RedirectToSignIn, SignedIn, SignedOut, useClerk } from "@clerk/clerk-react";
import App from "./App";
import React, { useEffect } from "react";

const rootElement = document.getElementById("root");
const root = createRoot(rootElement);

const clerkFrontendApi = process.env.REACT_APP_CLERK;

function AppWrapper() {
  const { signOut } = useClerk();

  useEffect(() => {
    const handleBeforeUnload = (event) => {
      signOut();
    };

    window.addEventListener("beforeunload", handleBeforeUnload);

    return () => {
      window.removeEventListener("beforeunload", handleBeforeUnload);
    };
  }, [signOut]);

  return <App />;
}

root.render(
  <ClerkProvider publishableKey={clerkFrontendApi}>
    <SignedIn>
      <AppWrapper />
    </SignedIn>
    <SignedOut>
      <RedirectToSignIn />
    </SignedOut>
  </ClerkProvider>
);